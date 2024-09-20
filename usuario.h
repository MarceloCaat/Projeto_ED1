#include <stdio.h>

#ifndef USUARIO_H
#define USUARIO_H
typedef struct {
    char username[80];
    char password[80];
} User;
void registrar_usuario();
int verificarEmailExistente(const char *username);
int verificarEmail(const char *username);
int autenticar_admin();
int login();
#endif