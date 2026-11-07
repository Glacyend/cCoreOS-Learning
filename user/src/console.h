/**
 * @file console.h
 * @brief User-side formatted output.
 */

#ifndef __USER_CONSOLE_H_
#define __USER_CONSOLE_H_

void user_printf(const char* fmt, ...);
void user_puts(const char* s);

#define printf(fmt, ...) user_printf(fmt, ##__VA_ARGS__)
#define puts(s) user_puts(s)

#endif /* __USER_CONSOLE_H_ */
