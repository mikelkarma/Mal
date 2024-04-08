#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <curl/curl.h>

#define MAX_PATH_LEN 1024
#define MAX_FILE_SIZE 78643200 // 75 MB em bytes

// Função para enviar arquivo via HTTP POST usando libcurl
int sendFile(char *filePath, char *url) {
    CURL *curl;
    CURLcode res;
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    // Inicializa o objeto CURL
    curl = curl_easy_init();
    if (curl) {
        // Adiciona o arquivo ao formulário para enviar via POST
        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "file",
                     CURLFORM_FILE, filePath,
                     CURLFORM_END);

        // Define a URL de destino
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Envia o formulário
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        // Realiza a requisição HTTP
        res = curl_easy_perform(curl);

        // Verifica se ocorreu algum erro
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Limpa o formulário
        curl_formfree(formpost);

        // Limpa o objeto CURL
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return (int)res;
}

// Função para verificar o tamanho de um arquivo
long int getFileSize(char *filePath) {
    struct stat st;
    if (stat(filePath, &st) == 0)
        return st.st_size;
    else
        return -1;
}

// Função para percorrer recursivamente a pasta e enviar arquivos menores que 75 MB
void sendFilesInDir(char *dirPath, char *url) {
    DIR *dir;
    struct dirent *entry;
    char filePath[MAX_PATH_LEN];

    // Abre o diretório
    dir = opendir(dirPath);
    if (!dir) {
        perror("Erro ao abrir diretório");
        return;
    }

    // Percorre os arquivos no diretório
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Se for um arquivo regular
            snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);
            long int fileSize = getFileSize(filePath);
            if (fileSize != -1 && fileSize < MAX_FILE_SIZE) {
                printf("Enviando arquivo: %s\n", filePath);
                if (sendFile(filePath, url) != 0)
                    printf("Falha ao enviar arquivo: %s\n", filePath);
            }
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) { // Se for um diretório
            // Chama recursivamente para percorrer subdiretórios
            snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);
            sendFilesInDir(filePath, url);
        }
    }

    // Fecha o diretório
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <caminho_para_pasta> <URL_do_servidor_PHP>\n", argv[0]);
        return 1;
    }

    char *dirPath = argv[1];
    char *url = argv[2];

    sendFilesInDir(dirPath, url);

    return 0;
}
