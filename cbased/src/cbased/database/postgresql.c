#include "cbased/database/postgresql.h"

static PGconn *pg_connect;

void
DB_pg_connect(const char *connect_info)
{
  pg_connect = PQconnectdb(connect_info);
}

b32
DB_pg_connect_is_valid(void)
{
  if (PQstatus(pg_connect) != CONNECTION_OK) {
    LOG_ERR("Connection to database failed: %s\n", PQerrorMessage(pg_connect));
    DB_pg_finish();
    return false;
  }

  return true;
}

void
DB_pg_finish(void)
{
  PQfinish(pg_connect);
}

PGresult *
DB_pg_exec(const char *query)
{
  PGresult *result = PQexec(pg_connect, query);

  // Проверка статуса выполнения запроса
  if (PQresultStatus(result) != PGRES_TUPLES_OK) {
    LOG_ERR("Query execution failed: %s\n", PQerrorMessage(pg_connect));
    PQclear(result);
    return NULL;
  }

  return result;
}
