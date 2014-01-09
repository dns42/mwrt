#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <mcc/lua.h>

#include <mcc/mcc.h>
#include <crt/log.h>

#include <stdlib.h>
#include <assert.h>

static void *
__lua_realloc(void *priv,
              void *p, size_t prev, size_t next)
{
    if (!next) {
        free(p);
        return NULL;
    }

    return realloc(p, next);
}

static int
__lua_panic(struct lua_State *L)
{
    const char *msg;

    msg = lua_tostring(L, -1);
    info("%s", msg);
    mcc_stop(g_mcc, EXIT_FAILURE, msg);

    return 0;
}

static int
__lua_print(lua_State* L)
{
    int rc, n, i;
    char *S, *pos;

    S = pos = NULL;
    rc = 0;

    n = lua_gettop(L);

    lua_getglobal(L, "tostring");

    for (i = 1; i <= n; i++) {
        const char *s;
        size_t l;

        lua_pushvalue(L, -1);
        lua_pushvalue(L, i);

        lua_call(L, 1, 1);

        s = lua_tolstring(L, -1, &l);
        if (!s) {
            const char *msg =
                LUA_QL("tostring")
                " must return a string to " LUA_QL("print");
            rc = luaL_error(L, msg);
            break;
        }

        if (l) {
            char *tmp;

            tmp = realloc(S, (pos - S) + (i > 1) + l + 1);
            if (!expected(tmp))
                break;

            pos = tmp + (pos - S);
            S = tmp;

            if (i > 1) {
                *pos = '\t';
                pos += 1;
            }

            strncpy(pos, s, l + 1);
            pos += l;
        }

        lua_pop(L, 1);
    }

    if (S) {
        info("%s", S);
        free(S);
    }

    return rc;
}

static const struct luaL_reg base_f [] = {
    { "print", __lua_print },
    { NULL, NULL }
};

static int
lua_libinit(struct lua_State *L)
{
    const lua_CFunction *fnp, init[] = {
        luaopen_bit32,
        luaopen_object,
        luaopen_event,
        luaopen_timer,
        luaopen_joystick,
        luaopen_netrx,
        luaopen_nettx,
        luaopen_rcvec,
        luaopen_msp,
        luaopen_rmi,
        luaopen_mcc,
        NULL,
    };
    lua_CFunction fn;

    info("Initializing %s", LUA_RELEASE);

    luaL_openlibs(L);

    lua_getglobal(L, "_G");
    luaL_register(L, NULL, base_f);
    lua_pop(L, 1);

    for (fnp = init; fn = *fnp, fn != NULL; fnp++) {
        fn(L);
        lua_settop(L, 0);
    }

    return 0;
}

struct lua_State *
lua_create(void)
{
    struct lua_State *L;
    int rc;

    L = lua_newstate(__lua_realloc, NULL);

    rc = expected(L) ? 0 : -1;
    if (rc)
        goto out;

    lua_atpanic(L, __lua_panic);

    luaL_openlibs(L);

    rc = lua_libinit(L);
out:
    if (rc && L) {
        lua_destroy(L);
        L = NULL;
    }

    return L;
}

void
lua_destroy(struct lua_State *L)
{
    lua_close(L);
}

/*
 * Local variables:
 * mode: C
 * c-file-style: "Linux"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
