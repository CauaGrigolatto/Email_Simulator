#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #define CLEAN_TERMINAL "cls"
#else
    #define CLEAN_TERMINAL "clear"
#endif

typedef struct Message {
    char authorEmail[100];
    char textContent[500];
    bool isRead;
} Message;

typedef struct User {
    char name[100];
    char email[100];
    char password[100];
    Message *inbox;
    int totalMessages;
} User;

void listMessages(Message inbox[], int totalMessages);
void readMessage(Message *message);
void showMessagePreview(char message[]);
int getTotalUnreadMessages(Message inbox[], int totalMessages);
Message createMessage(char authorEmail[100]);

User createUser();
int getIndexByEmail(char email[], User users[], int totalUsers);
bool authenticateUser(char email[], char password[], User users[], int totalUsers);
bool areStringsEqual(char string_1[], char string_2[]);
void clearBuffer();

void exibitHiddenPsw(char password[]);
int len(char word[]);
void showLogoAndColorfulMessage(char colorfulMessage[], int messageColor);
void showLogo();
void showColorfulMessage(char message[], int color);
void blueMessage(char message[]);
void greenMessage(char message[]);
void redMessage(char message[]);

int main() {
    int decision;

    User *users = (User*) malloc(sizeof(User));
    int totalUsers = 0; 
    int userIndex, messageIndex;

    User *onlineUser;
    char email[100];
    char password[100];

    bool emailExists, isUserAuthenticated;

    char colorfulMessage[100] = "Seja bem-vindo(a) ao Email Simulator!\n\n";
    int messageColor = 1;

    while (decision != 3) {  
        system(CLEAN_TERMINAL);

        onlineUser = NULL;

        showLogoAndColorfulMessage(colorfulMessage, messageColor);

        printf("(1)Entrar \u25CF ");
        printf("(2)Criar conta \u25CF ");
        printf("(3)Sair\n\n");

        scanf("%d", &decision);
        clearBuffer();

        system(CLEAN_TERMINAL);

        if (decision == 1) {
            showLogoAndColorfulMessage("Entrar em uma conta\n\n", 1);

            printf("Email: ");
            scanf("%[^\n]", email);
            clearBuffer();
        
            printf("Senha: ");
            scanf("%[^\n]", password);
            clearBuffer();

            isUserAuthenticated = authenticateUser(email, password, users, totalUsers);

            if (isUserAuthenticated) {
                userIndex = getIndexByEmail(email, users, totalUsers);
                onlineUser = &users[userIndex];
            }
            else {
                strcpy(colorfulMessage, "Email ou senha incorretos!\n\n"); 
                messageColor = 3;
            }
        }
        else if (decision == 2) {
            showLogoAndColorfulMessage("Criar uma conta\n\n", 1);

            User newUser = createUser();

            userIndex = getIndexByEmail(newUser.email, users, totalUsers);
            emailExists = userIndex != -1;

            if (! emailExists) {
                totalUsers++;
                User *resize = (User*) realloc(users, totalUsers*sizeof(User));

                if (resize) {
                    users = resize;
                    userIndex = totalUsers-1;
                    users[userIndex] = newUser;

                    strcpy(colorfulMessage, "Conta criada com sucesso!\n\n");
                    messageColor = 2;
                }   
                else {
                    strcpy(colorfulMessage, "(MEMORY ALLOC ERR) Falha ao criar novo usuário!\n\n");
                    messageColor = 3;
                }   
            } 
            else {
                strcpy(colorfulMessage, "Esse email já está em uso!\n\n");
                messageColor = 3;
            }            
        }
        else if (decision == 3) {
            blueMessage("Bye :)\n\n");
        }

        if (onlineUser != NULL) {
            strcpy(colorfulMessage, "Seja bem-vindo(a), ");
            strcat(colorfulMessage, onlineUser->name);
            strcat(colorfulMessage, "\n\n");
            messageColor = 1;

            system(CLEAN_TERMINAL);

            int onlineUserDecision = 0;

            while (onlineUserDecision != 4) {  
                system(CLEAN_TERMINAL);

                int unreadMessages = 
                getTotalUnreadMessages(onlineUser->inbox, onlineUser->totalMessages);
                
                showLogoAndColorfulMessage(colorfulMessage, messageColor);
                
                printf("(1)Caixa de entrada [%d] \u25CF ", unreadMessages);
                printf("(2)Nova mensagem \u25CF ");
                printf("(3)Editar conta \u25CF ");
                printf("(4)Sair da conta\n\n");
                scanf("%d", &onlineUserDecision);
                clearBuffer();

                system(CLEAN_TERMINAL); 

                if (onlineUserDecision == 1) {                
                    if (onlineUser->totalMessages > 0) {
                        showLogoAndColorfulMessage("Caixa de entrada | Pressione um número para abrir a mensagem\n\n", 1);

                        listMessages(onlineUser->inbox, onlineUser->totalMessages);
                        printf("\n");
                        scanf("%d", &messageIndex);
                        clearBuffer();

                        system(CLEAN_TERMINAL);

                        messageIndex = messageIndex - 1;
                        
                        if ((0 <= messageIndex) && (messageIndex < onlineUser->totalMessages)) {
                            showLogoAndColorfulMessage("Abrindo mensagem\n\n", 1);

                            readMessage(&onlineUser->inbox[messageIndex]);
                            getchar();

                            messageColor = 1;
                            strcpy(colorfulMessage, "O que acha de mandar umas mensagens?\n\n");
                        }
                        else {
                            messageColor = 3;
                            strcpy(colorfulMessage, "Esta mensagem não existe!\n\n");
                        }
                    }
                    else {
                        messageColor = 3;
                        strcpy(colorfulMessage, "Você ainda não possui mensagens!\n\n");
                    }
                }
                else if (onlineUserDecision == 2) {
                    messageColor = 1;
                    strcpy(colorfulMessage, "Nova mensagem\n\n");

                    showLogoAndColorfulMessage(colorfulMessage, messageColor);

                    char targetEmail[100];
                    
                    printf("Email destinatário: ");
                    scanf("%[^\n]", targetEmail);
                    clearBuffer();

                    printf("\n\n");

                    userIndex = getIndexByEmail(targetEmail, users, totalUsers);
                    emailExists = userIndex != -1;

                    if (emailExists) {

                        bool isToItself = areStringsEqual(users[userIndex].email, onlineUser->email);

                        if (! isToItself) {
                            Message message = createMessage(onlineUser->email);

                            User *targetUser = &users[userIndex];
                            
                            Message *resize = 
                            realloc(targetUser->inbox, (targetUser->totalMessages+1) * sizeof(Message));

                            if (resize) {
                                targetUser->inbox = resize;
                                targetUser->totalMessages++;

                                messageIndex = targetUser->totalMessages-1;
                                
                                targetUser->inbox[messageIndex] = message;
                                
                                messageColor = 2;
                                strcpy(colorfulMessage, "Mensagem enviada com sucesso!\n\n");
                            }
                            else {
                                messageColor = 3;
                                strcpy(colorfulMessage, "(MEMORY ALLOC ERR) Falha ao enviar mensagem!\n\n");
                            }
                        }
                        else {
                            messageColor = 3;
                            strcpy(colorfulMessage, "Não é possível enviar mensagens a si mesmo(a)!\n\n");
                        }

                    }
                    else {
                        messageColor = 3;
                        strcpy(colorfulMessage, "Email inválido!\n\n");
                    }

                }
                else if (onlineUserDecision == 3) {
                    strcpy(colorfulMessage, "Atualize as informações do perfil\n\n");
                    messageColor = 1;

                    

                    int updatingAccountDecision = 0;
                    
                    while (updatingAccountDecision != 4) {

                        bool isCorrectPassword;

                        system(CLEAN_TERMINAL);
                        showLogoAndColorfulMessage(colorfulMessage, messageColor);

                        printf("Nome: %s \u25CF ", onlineUser->name);
                        printf("Email: %s \u25CF ", onlineUser->email);
                        printf("Senha: "); exibitHiddenPsw(onlineUser->password);
                        printf("\n\n");
                        
                        printf("(1)Mudar nome \u25CF ");
                        printf("(2)Mudar email \u25CF ");
                        printf("(3)Mudar senha \u25CF ");
                        printf("(4)Voltar\n\n");
                        scanf("%d", &updatingAccountDecision);
                        clearBuffer();
                        printf("\n");

                        if (updatingAccountDecision == 1) {
                            strcpy(colorfulMessage, "Mudar nome\n\n");
                            messageColor = 1;

                            char newName[100];
                            printf("Novo nome: ");
                            scanf("%[^\n]", newName);

                            if (areStringsEqual(newName, onlineUser->name)) {
                                strcpy(colorfulMessage, "Os nomes devem ser diferentes!\n\n"),
                                messageColor = 3;
                            }
                            else {
                                strcpy(colorfulMessage, "Nome alterado com sucesso!\n\n"),
                                messageColor = 2;
                                strcpy(onlineUser->name, newName);
                            }
                        }
                        else if (updatingAccountDecision == 2) {
                            strcpy(colorfulMessage, "Mudar email\n\n");
                            messageColor = 1;

                            char newEmail[100];
                            printf("Novo email: ");
                            scanf("%[^\n]", newEmail);
                            clearBuffer();

                            if (areStringsEqual(newEmail, onlineUser->email)) {
                                strcpy(colorfulMessage, "Os emails devem ser diferentes!\n\n"),
                                messageColor = 3;
                            }
                            else {
                                emailExists = getIndexByEmail(newEmail, users, totalUsers) != -1;

                                if (emailExists) {
                                    strcpy(colorfulMessage, "Este email já está sendo usado!\n\n"),
                                    messageColor = 3;
                                }
                                else {
                                    printf("Informe sua senha para confirmar alteração: ");
                                    scanf("%[^\n]", password);
                                    clearBuffer();

                                    isCorrectPassword = areStringsEqual(password, onlineUser->password);                    

                                    if (isCorrectPassword) {
                                        strcpy(colorfulMessage, "Email alterado com sucesso!\n\n"),
                                        messageColor = 2;
                                        strcpy(onlineUser->email, newEmail);
                                    }
                                    else {
                                        strcpy(colorfulMessage, "Senha incorreta!\n\n"),
                                        messageColor = 3;
                                    }

                                }
                            }
                        }
                        else if (updatingAccountDecision == 3) {
                            strcpy(colorfulMessage, "Mudar senha\n\n");
                            messageColor = 1;

                            printf("Senha antiga: ");
                            scanf("%[^\n]", password);
                            clearBuffer();

                            char newPassword[100];
                            printf("Nova senha: ");
                            scanf("%[^\n]", newPassword);

                            isCorrectPassword = areStringsEqual(password, onlineUser->password);

                            if (isCorrectPassword) {
                                if (areStringsEqual(newPassword, onlineUser->password)) {
                                    strcpy(colorfulMessage, "As senhas devem ser diferentes!\n\n"),
                                    messageColor = 3;
                                }
                                else {
                                    strcpy(colorfulMessage, "Senha alterado com sucesso!\n\n"),
                                    messageColor = 2;
                                    strcpy(onlineUser->password, newPassword);
                                } 
                            }
                            else {
                                strcpy(colorfulMessage, "Senha incorreta!\n\n"),
                                messageColor = 3;
                            }
                        }
                        else if (updatingAccountDecision == 4) {
                            messageColor = 1;
                            strcpy(colorfulMessage, "Cheque sua caixa de entrada para não perder nenhuma mensagem\n\n");
                        }
                        else {
                            messageColor = 3;
                            strcpy(colorfulMessage, "Opção inválida!\n\n");
                        }
                    }
                }
                else if (onlineUserDecision == 4) {
                    messageColor = 3;
                    strcpy(colorfulMessage, "Saiu da conta!\n\n");
                }
            }
        }    
    }

    free(users);

    return EXIT_SUCCESS;
}

void exibitHiddenPsw(char password[]) {
    int wordLen = len(password);

    for (int i = 0; i < wordLen; i++) {
        printf("*");
    }
}

int len(char word[]) {
    int i = 0;
    while (word[i] != '\0') {
        i++;
    }
    return i;
}

//Aplicar recursividade aqui: (Aplicado)
void listMessages(Message inbox[], int totalMessages) {
    totalMessages = totalMessages - 1;

    if (totalMessages >= 0) {
        printf("%d- %s ", (totalMessages+1), inbox[totalMessages].authorEmail);

        inbox[totalMessages].isRead ? printf("\n") : blueMessage("(Nova)\n");

        showMessagePreview(inbox[totalMessages].textContent);
        printf("\n\n");

        listMessages(inbox, totalMessages);
    }

}

void readMessage(Message *message) {
    message->isRead = true;

    printf("De: %s\n\n", message->authorEmail);
    printf("Mensagem:\n\n");
    printf("%s\n\n", message->textContent);
    
}

void showMessagePreview(char message[]) {
    int i = 0;

    while ((i < 30) && (message[i] != '\0')) {
        printf("%c", message[i]);
        i++;        
    }

    if (i == 30) {
        printf("...\n");
    }
}

// Aplicar recursividade aqui: ()
int getTotalUnreadMessages(Message inbox[], int totalMessages) {
    totalMessages = totalMessages - 1;
    int unreadMessages = 0;
    
    if (totalMessages >= 0) {
        if (! inbox[totalMessages].isRead) {
            unreadMessages++;
        } 
        unreadMessages += getTotalUnreadMessages(inbox, totalMessages);
    }

    return unreadMessages;
}

Message createMessage(char authorEmail[100]) {
    Message message;
    strcpy(message.authorEmail, authorEmail);

    printf("Mensagem:\n\n");
    scanf("%[^\n]", message.textContent);

    message.isRead = false;

    return message;
}

int getIndexByEmail(char email[], User users[], int totalUsers) {
    int i = 0;
    int index = -1;

    while ((i < totalUsers) && (index == -1)) {
        if (areStringsEqual(email, users[i].email)) {
            index = i;
        }
        i++;    
    }

    return index;
}

bool authenticateUser(char email[], char password[], User users[], int totalUsers) {
    bool authenticated = false;
    int indexOfEmail = getIndexByEmail(email, users, totalUsers);
    if (indexOfEmail != -1) {
        if (areStringsEqual(users[indexOfEmail].password, password)) {
            authenticated = true;
        }    
    }

    return authenticated;
}

User createUser() {
    User user;
    char psw[100];

    printf("Nome: ");
    scanf("%[^\n]", user.name);
    clearBuffer();

    printf("Email: ");
    scanf("%[^\n]", user.email);
    clearBuffer();

    printf("Senha: ");
    scanf("%[^\n]", user.password);
    clearBuffer();

    user.inbox = (Message*) malloc(sizeof(Message));
    user.totalMessages = 0;

    return user;
}

bool areStringsEqual(char string_1[], char string_2[]) {
    return strcmp(string_1, string_2) == 0;
}

void clearBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void showLogoAndColorfulMessage(char colorfulMessage[], int messageColor) {
    showLogo();
    showColorfulMessage(colorfulMessage, messageColor);
}

void showLogo() {
    printf("8\"\"\"\"\"                           8\"\"\"\"\"8                                                 \n");
    printf("8     eeeeeee eeeee e  e        8      e  eeeeeee e   e e     eeeee eeeee eeeee eeeee  \n");
    printf("8eeee 8  8  8 8   8 8  8        8eeeee 8  8  8  8 8   8 8     8   8   8   8  88 8   8  \n");
    printf("88    8e 8  8 8eee8 8e 8e           88 8e 8e 8  8 8e  8 8e    8eee8   8e  8   8 8eee8e \n");
    printf("88    88 8  8 88  8 88 88       e   88 88 88 8  8 88  8 88    88  8   88  8   8 88   8 \n");
    printf("88eee 88 8  8 88  8 88 88eee    8eee88 88 88 8  8 88ee8 88eee 88  8   88  8eee8 88   8 \n");
    printf("                                                                                       \n");
}

void showColorfulMessage(char message[], int color) {
    switch (color) {
    case 3:
        redMessage(message);
        break;
    case 2:
        greenMessage(message);
        break;
    default:
        blueMessage(message);
        break;
    }
}

void blueMessage(char message[]) {
    printf("\033[1;34m%s\033[0m", message);
}

void greenMessage(char message[]) {
    printf("\033[1;32m%s\033[0m", message);
}

void redMessage(char message[]){
    printf("\033[1;31m%s\033[0m", message);
}