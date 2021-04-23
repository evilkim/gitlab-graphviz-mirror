#pragma once

#ifdef _WIN32
#ifdef EXPORT_CGRAPH
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif
#else
#define DECLSPEC /* nothing */
#endif

#ifdef __clang__
#define NONNULL _Nonnull
#define NULLABLE _Nullable
#else
#define NONNULL /* nothing */
#define NULLABLE /* nothing */
#endif

#ifdef __GNUC__
#define PRINTF_LIKE(fmt, args) __attribute__((format(printf, fmt, args)))
#else
#define PRINTF_LIKE(fmt, args) /* nothing */
#endif

#ifdef __GNUC__
#if defined(__clang__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
#define RETURNS_NONNULL __attribute__((returns_nonnull))
#else
#define RETURNS_NONNULL /* nothing */
#endif
#else
#define RETURNS_NONNULL /* nothing */
#endif

#ifdef __GNUC__
#define WUR __attribute__((warn_unused_result))
#else
#define WUR
#endif

/** sprintf-alike
 *
 * This function constructs a dynamically allocated string based on its
 * printf-style arguments and returns a pointer to this string. It is intended
 * to be used as a safe alternative to sprintf.
 *
 * @param format Printf-style format string
 * @param ... Format arguments, if any
 * @returns A pointer to the constructed string or NULL on failure
 */
DECLSPEC PRINTF_LIKE(1, 2) WUR
char *NULLABLE gv_sprint(const char *NONNULL format, ...);

/** gv_sprint wrapper, calling exit if NULL is returned
 *
 * This alternative is provided for callers who have no reasonable way to handle
 * a NULL return from gv_sprint. However, it is always preferable to call
 * gv_sprint and handle the error gracefully for composability.
 *
 * @param format Printf-style format string
 * @param ... Format arguments, if any
 * @returns A pointer to the constructed string
 */
DECLSPEC PRINTF_LIKE(1, 2) RETURNS_NONNULL WUR
char *NONNULL gv_sprint_or_exit(const char *NONNULL format, ...);

#undef WUR
#undef RETURNS_NONNULL
#undef PRINTF_LIKE
#undef NONNULL
#undef NULLABLE
#undef DECLSPEC
