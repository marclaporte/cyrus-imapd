/* api.h -- replication-based backup api
 *
 * Copyright (c) 1994-2015 Carnegie Mellon University.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name "Carnegie Mellon University" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For permission or any legal
 *    details, please contact
 *      Carnegie Mellon University
 *      Center for Technology Transfer and Enterprise Creation
 *      4615 Forbes Avenue
 *      Suite 302
 *      Pittsburgh, PA  15213
 *      (412) 268-7393, fax: (412) 268-7395
 *      innovation@andrew.cmu.edu
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Computing Services
 *     at Carnegie Mellon University (http://www.cmu.edu/computing/)."
 *
 * CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef BACKUP_API_H
#define BACKUP_API_H

#include "imap/dlist.h"
#include "imap/mboxname.h"

struct backup;

struct backup *backup_open(const char *name, off_t *opened_at);
int backup_close(struct backup **backupp);

int backup_reindex(const char *name);

int backup_write_dlist(struct backup *backup, time_t ts, struct dlist *dl);
int backup_index_dlist(struct backup *backup, struct dlist *dl, off_t dl_offset, size_t dl_len);

int backup_index_start(struct backup *backup, time_t ts, off_t offset);
int backup_index_end(struct backup *backup, size_t length);

/* FIXME new api? */

struct backup_message {
    int id;
    struct message_guid *guid;
    char *partition;
    int backup_id;
    off_t offset;
    size_t length;
};

struct backup_mailbox {
    int id;
    int last_backup_id;
    int deleted;
    struct dlist *dlist;
};

typedef int (*backup_mailbox_foreach_cb)(const struct backup_mailbox *mailbox,
                                         void *rock);
int backup_mailbox_foreach(struct backup *backup, int want_records,
                           backup_mailbox_foreach_cb cb, void *rock);
struct backup_mailbox *backup_get_mailbox_by_name(struct backup *backup, const mbname_t *mbname, int want_records);
void backup_mailbox_free(struct backup_mailbox **mailbox);

int backup_get_message_id(struct backup *backup, const char *guid);
struct backup_message *backup_get_message(struct backup *backup, const struct message_guid *guid);
char *backup_get_message_content(struct backup *backup, const struct backup_message *message);
void backup_message_free(struct backup_message **message);

int backup_append_start(struct backup *backup, time_t ts);
int backup_append(struct backup *backup, struct dlist *dlist, time_t ts);
int backup_append_done(struct backup *backup);

int backup_index_start(struct backup *backup, time_t ts, off_t offset);
int backup_index(struct backup *backup, struct dlist *dlist, time_t ts);
int backup_index_done(struct backup *backup, size_t len);

int backup_close(struct backup **backupp); // also ends index/append ops

/******************/
#endif