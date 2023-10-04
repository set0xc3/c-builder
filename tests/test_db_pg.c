#include <cbased.h>

enum {
  Client_Id      = 1,
  Client_Name    = 2,
  Client_Phone   = 3,
  Client_Address = 4,
  Client_Email   = 5,
  Client_Date    = 6,
};

int
main(void)
{
  // IP Docker postgresql (172.17.0.2)
  DB_pg_connect("host=172.17.0.2 port=5432 dbname=test user=postgres "
                "password=root connect_timeout=10");

  if (!DB_pg_connect_is_valid()) {
    DB_pg_finish();
    return 1;
  }

  PGresult *pg_result = DB_pg_exec("SELECT * FROM clients");
  if (!pg_result) {
    DB_pg_finish();
    return 1;
  }

  // Получение количества строк и столбцов в результате запроса
  int rows = PQntuples(pg_result);
  int cols = PQnfields(pg_result);

  // Вывод информации о каждом клиенте
  for (int i = 0; i < rows; i++) {
    LOG_INFO("Client ID: %s\n", PQgetvalue(pg_result, i, Client_Id));
    LOG_INFO("Name: %s\n", PQgetvalue(pg_result, i, Client_Name));
    LOG_INFO("Phone: %s\n", PQgetvalue(pg_result, i, Client_Phone));
    LOG_INFO("Address: %s\n", PQgetvalue(pg_result, i, Client_Address));
    LOG_INFO("Email: %s\n", PQgetvalue(pg_result, i, Client_Email));
    LOG_INFO("Date: %s\n", PQgetvalue(pg_result, i, Client_Date));
    LOG_INFO("\n");
  }

  DB_pg_finish();

  return 0;
}
