#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"


static int ShowMainMenu();
static void ShowProductsMenu();
static int ShowProductsSubMenu();


int products_query_1();
int products_query_2();

/* #endif //A2BIS_MENU_TEMPLATE_H */

/**
 * @file menu.c
 * @author Sergio Sanchez y Roberto Manzaneque
 * @date 25 Abril 2020
 * @brief File containing an example of a menu system with submenus
 *
 * The following program allows a user to select a nursery rhyme
 * or a fairy tail from a menu. The menu has two levels, the top
 * one allows to select between fairy tails or nursery rhymes. The
 * second level (sub-menus) allow to select either a particular
 * rhyme or tail.
 *
 * @see https://bytes.com/topic/c/answers/567136-creating-menus-sub-menus
 */


/**
 * @brief Entry point, shows and process main menu.
 *
 * This function implements a loop that prints the main
 * menu, analyze the user selection and calls the
 * corresponding submenu
 *
 * @return 0 if no error
 * @author rmarabini
 */
int main(void) {


    int nChoice = 0;

    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1: {
                ShowProductsMenu();
            }
                break;

            case 2: {

            }
                break;

            case 3: {

            }
                break;

            case 4: {
                printf("Adios!!\n\n");
                break;
                }

            }

        } while (nChoice != 4);


    return 0;

}




int ShowMainMenu() {
    int nSelected = 0;
    char buf[16]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do {
        printf("Programa de datos de cierto comercio.\n");
        printf("Elige una opción de las disponibles\n\n");

        printf(" (1) Products\n"
               " (2) Orders\n"
               " (3) Customers\n"
               " (4) Exit\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("Opción incorrecta.\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

void ShowProductsMenu() {
    int nChoice = 0;
    int ret= EXIT_FAILURE;

        do {
        nChoice = ShowProductsSubMenu();
        switch (nChoice) {

            case 1: {

            if(products_query_1() == ret) break;
            }
                break;

           case 2: {
            if(products_query_2() == ret) break;
            }
                break;

            case 3: {
            }
                break;

        }
    } while (nChoice != 3);

}

int products_query_1(){

  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN ret;
  char x[512];
  SQLCHAR y[512];
  char query[512];
  int i;
  int len;
  /*conexion*/
  ret = odbc_connect(&env, &dbc);
  if (!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
  }

  /* Allocate a statement handle */
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    printf("Enter productcode > ");
    fflush(stdout);
    fgets(x, sizeof(x), stdin);
    len = strlen(x);
    for(i=0;i<len;i++){

      if(x[i]=='\n'){
        x[i]= '\'';
        break;
      }
    }
      sprintf(query, "SELECT p.quantityinstock FROM products p WHERE  p.productcode =  \'%s",x);

      SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);



      SQLBindCol(stmt, 1, SQL_C_CHAR, y, sizeof(y), NULL);

      /* Loop through the rows in the result-set */
      if(SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
          printf("y = %s\n", y);
      }
          SQLCloseCursor(stmt);



      printf("\n");

      /* free up statement handle */
      SQLFreeHandle(SQL_HANDLE_STMT, stmt);

      /* DISCONNECT */
      ret = odbc_disconnect(env, dbc);
      if (!SQL_SUCCEEDED(ret)) {
          return EXIT_FAILURE;
      }

      return EXIT_SUCCESS;

  }


int products_query_2(){
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN ret;
  char x[512];
  SQLCHAR y[512];
  SQLCHAR z[512];
  char query[512];
  int i;
  int len;

  /*conexion*/
  ret = odbc_connect(&env, &dbc);
  if (!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
  }

  /* Allocate a statement handle */
  SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    printf("Enter productname > ");
    fflush(stdout);
    fgets(x, sizeof(x), stdin);

    len = strlen(x);
    for(i=0;i<len;i++){

      if(x[i]=='\n'){
        x[i]= '\0';
        break;
      }
    }



      sprintf(query, "select p.productcode, p.productname from products p where p.productname like \'%%%s%%\'",x);

      /*printf("%s\n", query);*/
      SQLExecDirect(stmt, (SQLCHAR*) query, SQL_NTS);

      SQLBindCol(stmt, 1, SQL_C_CHAR, y, sizeof(y), NULL);
      SQLBindCol(stmt, 2, SQL_C_CHAR, z, sizeof(z), NULL);
      /* Loop through the rows in the result-set */
      printf("y        z\n");
      while(SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
          printf("%s %s\n", y, z);

      }
          SQLCloseCursor(stmt);



      printf("\n");

      /* free up statement handle */
      SQLFreeHandle(SQL_HANDLE_STMT, stmt);

      /* DISCONNECT */
      ret = odbc_disconnect(env, dbc);
      if (!SQL_SUCCEEDED(ret)) {
          return EXIT_FAILURE;
      }

      return EXIT_SUCCESS;

}




int ShowProductsSubMenu() {
   int nSelected = 0;
   char buf[16];

   do {

       printf(" (1) Stock\n"
              " (2) Find\n"
              " (3) Back\n\n");

       printf("Enter a number that corresponds to your choice > ");
       if (!fgets(buf, 16, stdin))
           /* reading input failed, give up: */
           nSelected =0;
       else
           /* have some input, convert it to integer: */
           nSelected = atoi(buf);
       printf("\n");

       if ((nSelected < 1) || (nSelected > 3)) {
           printf("Opción incorrecta, introduce un número otra vez\n\n\n");
       }
   } while ((nSelected < 1) || (nSelected > 3));

   return nSelected;
}
