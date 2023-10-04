#include <cbased.h>

#include <libpq-fe.h>
#include <openssl/ssl.h>

int
main(void)
{
  PGconn *conn
      = PQconnectdb("host=172.17.0.2 user=postgres password=root dbname=dev");
  if (PQstatus(conn) == CONNECTION_OK) {
    LOG_INFO("Connected to the database!\n");
    LOG_INFO("Port: %s\n", PQport(conn));
    LOG_INFO("Host: %s\n", PQhost(conn));
    LOG_INFO("DBName: %s\n", PQdb(conn));
  } else {
    LOG_INFO("Connection failed: %s\n", PQerrorMessage(conn));
    PQfinish(conn);
    return 0;
  }

  PQfinish(conn);

  return 0;
}
