#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

struct memstream {
   FILE *file;
   char **buffer_out;
   size_t *size_out;
   int read_fd;
   pthread_t reader;
   pthread_mutex_t lock;
   pthread_cond_t changed;
   char *data;       /* always NUL-terminated */
   size_t length;    /* bytes written, not counting the NUL */
   size_t capacity;  /* bytes allocated, the NUL included */
   bool eof;
   bool failed;      /* the buffer could not grow */
   struct memstream *next;
};

static pthread_mutex_t streams_lock = PTHREAD_MUTEX_INITIALIZER;
static struct memstream *streams;

/* Appends what the pipe holds now; the stream's lock is held. */
static void
drain(struct memstream *stream)
{
   char chunk[16384];
   for (;;) {
      const ssize_t got = read(stream->read_fd, chunk, sizeof(chunk));
      if (got > 0) {
         if (!stream->failed && stream->length + (size_t)got + 1 > stream->capacity) {
            size_t capacity = stream->capacity * 2;
            while (capacity < stream->length + (size_t)got + 1)
               capacity *= 2;
            char *const grown = realloc(stream->data, capacity);
            if (grown) {
               stream->data = grown;
               stream->capacity = capacity;
            } else {
               stream->failed = true;
            }
         }
         if (!stream->failed) {
            memcpy(stream->data + stream->length, chunk, (size_t)got);
            stream->length += (size_t)got;
            stream->data[stream->length] = '\0';
         }
         continue;
      }
      if (got == 0)
         stream->eof = true;
      else if (errno == EINTR)
         continue;
      else if (errno != EAGAIN)
         stream->eof = true;
      return;
   }
}

static void *
reader_main(void *argument)
{
   struct memstream *const stream = argument;
   for (;;) {
      struct pollfd ready = {.fd = stream->read_fd, .events = POLLIN};
      if (poll(&ready, 1, -1) < 0 && errno != EINTR) {
         pthread_mutex_lock(&stream->lock);
         stream->eof = true;
         pthread_cond_broadcast(&stream->changed);
         pthread_mutex_unlock(&stream->lock);
         return NULL;
      }
      pthread_mutex_lock(&stream->lock);
      drain(stream);
      const bool done = stream->eof;
      pthread_cond_broadcast(&stream->changed);
      pthread_mutex_unlock(&stream->lock);
      if (done)
         return NULL;
   }
}

static bool
pipe_is_empty(int fd)
{
   struct pollfd ready = {.fd = fd, .events = POLLIN};
   return poll(&ready, 1, 0) == 0;
}

/* Waits until the buffer holds everything written so far, then gives it to
 * the caller; the stream's lock is held. */
static void
publish(struct memstream *stream)
{
   while (!stream->eof && !pipe_is_empty(stream->read_fd))
      pthread_cond_wait(&stream->changed, &stream->lock);
   *stream->buffer_out = stream->data;
   *stream->size_out = stream->length;
}

FILE *
open_memstream(char **buffer, size_t *size)
{
   if (!buffer || !size) {
      errno = EINVAL;
      return NULL;
   }
   struct memstream *const stream = calloc(1, sizeof(*stream));
   if (!stream)
      return NULL;
   stream->capacity = 256;
   stream->data = malloc(stream->capacity);
   int fds[2] = {-1, -1};
   if (!stream->data || pipe(fds) != 0)
      goto fail;
   stream->data[0] = '\0';
   stream->read_fd = fds[0];
   stream->buffer_out = buffer;
   stream->size_out = size;
   pthread_mutex_init(&stream->lock, NULL);
   pthread_cond_init(&stream->changed, NULL);
   if (fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK) != 0)
      goto fail_sync;
   stream->file = fdopen(fds[1], "w");
   if (!stream->file)
      goto fail_sync;
   fds[1] = -1;
   if (pthread_create(&stream->reader, NULL, reader_main, stream) != 0) {
      fclose(stream->file);
      goto fail_sync;
   }
   *buffer = stream->data;
   *size = 0;
   pthread_mutex_lock(&streams_lock);
   stream->next = streams;
   streams = stream;
   pthread_mutex_unlock(&streams_lock);
   return stream->file;

fail_sync:
   pthread_cond_destroy(&stream->changed);
   pthread_mutex_destroy(&stream->lock);
fail:
   {
      const int saved = errno;
      if (fds[0] >= 0)
         close(fds[0]);
      if (fds[1] >= 0)
         close(fds[1]);
      free(stream->data);
      free(stream);
      errno = saved;
   }
   return NULL;
}

static struct memstream *
find(FILE *file, bool unlink)
{
   pthread_mutex_lock(&streams_lock);
   struct memstream **link = &streams;
   while (*link && (*link)->file != file)
      link = &(*link)->next;
   struct memstream *const stream = *link;
   if (stream && unlink)
      *link = stream->next;
   pthread_mutex_unlock(&streams_lock);
   return stream;
}

int
__wrap_fflush(FILE *file)
{
   const int result = fflush(file);
   if (file) {
      struct memstream *const stream = find(file, false);
      if (stream) {
         pthread_mutex_lock(&stream->lock);
         publish(stream);
         pthread_mutex_unlock(&stream->lock);
      }
      return result;
   }
   /* fflush(NULL) flushes every stream: publish every memory stream. */
   pthread_mutex_lock(&streams_lock);
   for (struct memstream *stream = streams; stream; stream = stream->next) {
      pthread_mutex_lock(&stream->lock);
      publish(stream);
      pthread_mutex_unlock(&stream->lock);
   }
   pthread_mutex_unlock(&streams_lock);
   return result;
}

int
__wrap_fclose(FILE *file)
{
   struct memstream *const stream = file ? find(file, true) : NULL;
   const int result = fclose(file);
   if (!stream)
      return result;
   /* Closing the write end ends the reader, after it drains the pipe. */
   pthread_join(stream->reader, NULL);
   pthread_mutex_lock(&stream->lock);
   publish(stream);
   pthread_mutex_unlock(&stream->lock);
   close(stream->read_fd);
   pthread_cond_destroy(&stream->changed);
   pthread_mutex_destroy(&stream->lock);
   const int failed = stream->failed;
   /* The buffer is the caller's now. */
   free(stream);
   if (failed && result == 0) {
      errno = ENOMEM;
      return EOF;
   }
   return result;
}

