#ifndef SERIALTEST_H
#define SERIALTEST_H

int open_port(void);
void fetch_sentence_from_gps(int fd, char* buffer);

#endif /* SERIALTEST_H */
