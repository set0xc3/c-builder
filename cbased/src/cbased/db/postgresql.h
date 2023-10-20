#pragma once

#include "cbased/core/base.h"
#include "cbased/core/log.h"

#include <libpq-fe.h>

#ifdef __cplusplus
extern "C" {
#endif

api b32       DB_pg_connect(const char *connect_info);
api b32       DB_pg_connect_is_valid(void);
api void      DB_pg_finish(void);
api PGresult *DB_pg_exec(const char *query);

#ifdef __cplusplus
}
#endif
