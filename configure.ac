AC_PREREQ([2.63])
AC_INIT([ob-uspagent],[1.0.0])
AM_INIT_AUTOMAKE([foreign silent-rules subdir-objects -Wall -Werror -Wno-portability])
AC_CONFIG_MACRO_DIR([m4])
AC_USE_SYSTEM_EXTENSIONS

LT_PREREQ([2.2])
LT_INIT([shared disable-static])

AC_PROG_CC
AM_PROG_CC_C_O

AC_ARG_ENABLE(stomp, [AS_HELP_STRING([--enable-stomp], [enable STOMP Message support])],,enable_stomp=yes)
AC_ARG_ENABLE(mqtt, [AS_HELP_STRING([--enable-mqtt], [enable MQTT Message support])],,enable_mqtt=yes)
AC_ARG_ENABLE(coap, [AS_HELP_STRING([--enable-coap], [enable COAP Message support])],,enable_coap=yes)
AC_ARG_ENABLE(websockets, [AS_HELP_STRING([--enable-websockets], [enable WebSockets Message support])],,enable_websockets=yes)
AC_ARG_ENABLE(uds, [AS_HELP_STRING([--enable-uds], [enable UDS MTP Message support])],,enable_uds=yes)
AC_ARG_ENABLE(bulkdata, [AS_HELP_STRING([--enable-bulkdata], [enable Bulk Data Collection support])],,enable_bulkdata=yes)
AC_ARG_ENABLE(hardening, [AS_HELP_STRING([--enable-hardening], [enable compiler hardening flags])],,enable_hardening=no)
AC_ARG_ENABLE(asan, [AS_HELP_STRING([--enable-asan], [enable address sanitizer])],,enable_asan=no)
AC_ARG_ENABLE(tsan, [AS_HELP_STRING([--enable-tsan], [enable thread sanitizer])],,enable_tsan=no)

PKG_CHECK_MODULES([sqlite3], [sqlite3])
PKG_CHECK_MODULES([zlib], [zlib])

# --- Fallback para libuci ---
PKG_CHECK_MODULES([libuci], [libuci], [], [
    AC_CHECK_LIB([uci], [uci_alloc_context], [
        libuci_LIBS="-luci"
        AC_SUBST([libuci_LIBS])
    ], [
        AC_MSG_ERROR([libuci not found])
    ])
])

AS_IF([test "x$enable_stomp" = "xno"], [
	AC_DEFINE(DISABLE_STOMP)
], [
    PKG_CHECK_MODULES([openssl], [openssl])
])

AS_IF([test "x$enable_coap" = "xyes"], [
	AC_DEFINE(ENABLE_COAP)
    PKG_CHECK_MODULES([openssl], [openssl])
])

AS_IF([test "x$enable_mqtt" = "xyes"], [
	PKG_CHECK_MODULES([libmosquitto], [libmosquitto])
	AC_DEFINE(ENABLE_MQTT)
    PKG_CHECK_MODULES([openssl], [openssl])
])

AS_IF([test "x$enable_websockets" = "xyes"], [
    PKG_CHECK_MODULES([libwebsockets], [libwebsockets >= 4.2.0])
	AC_DEFINE(ENABLE_WEBSOCKETS)
    PKG_CHECK_MODULES([openssl], [openssl])
])

AS_IF([test "x$enable_uds" = "xyes"], [
	AC_DEFINE(ENABLE_UDS)
])

AS_IF([test "x$enable_bulkdata" = "xyes"], [
    PKG_CHECK_MODULES([libcurl], [libcurl])
    PKG_CHECK_MODULES([openssl], [openssl])
], [
	AC_DEFINE(REMOVE_DEVICE_BULKDATA)
])

AS_IF([test "x$enable_hardening" = "xyes"], [
    CFLAGS_HARDENING="-fstack-protector-strong -fPIE"
    LDFLAGS_HARDENING="-pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack"
    CFLAGS_HARDENING="$CFLAGS_HARDENING -O2 -D_FORTIFY_SOURCE=2"
    AC_SUBST([CFLAGS_HARDENING])
    AC_SUBST([LDFLAGS_HARDENING])
])

if test "x$enable_asan" = "xyes" && test "x$enable_tsan" = "xyes"; then
    AC_MSG_ERROR([--enable-asan and --enable-tsan are mutually exclusive.])
fi

AS_IF([test "x$enable_asan" = "xyes"], [
    CPPFLAGS_SANITIZER="-Wno-stringop-truncation"
    CFLAGS_SANITIZER="-g -O1 -fsanitize=address -fno-omit-frame-pointer -fno-common"
    LDFLAGS_SANITIZER="-fsanitize=address"
    AC_SUBST([CPPFLAGS_SANITIZER])
    AC_SUBST([CFLAGS_SANITIZER])
    AC_SUBST([LDFLAGS_SANITIZER])
])

AS_IF([test "x$enable_tsan" = "xyes"], [
    CPPFLAGS_SANITIZER="-Wno-stringop-truncation"
    CFLAGS_SANITIZER="-g -O2 -fsanitize=thread -fno-omit-frame-pointer -fno-common"
    LDFLAGS_SANITIZER="-fsanitize=thread"
    AC_SUBST([CPPFLAGS_SANITIZER])
    AC_SUBST([CFLAGS_SANITIZER])
    AC_SUBST([LDFLAGS_SANITIZER])
])

AC_FUNC_STRERROR_R
AC_CHECK_HEADERS([malloc.h execinfo.h])
AC_CHECK_FUNCS([mallinfo mallinfo2])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT