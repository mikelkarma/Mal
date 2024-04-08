

```markdown
# Enviador de Arquivos em C para Servidor PHP

Este é um programa simples em C que percorre uma pasta e suas subpastas, enviando todos os arquivos com menos de 75 MB para um servidor PHP usando a biblioteca libcurl.

## Requisitos

- GCC (GNU Compiler Collection)
- Biblioteca libcurl (libcurl4-openssl-dev no Linux)

## Compilação

Para compilar o programa, execute o seguinte comando no terminal:


gcc -march=arquitetura -o send_files send_files.c -lcurl
```

## Uso

Execute o programa com os seguintes argumentos:

```
./send_files <caminho_para_pasta> <URL_do_servidor_PHP>
```

Por exemplo:

```
./send_files /caminho/para/minha/pasta http://exemplo.com/upload.php
```

Certifique-se de que o servidor PHP esteja configurado para receber arquivos via POST e lidar com eles corretamente.

## Exemplo de Script PHP para Receber os Arquivos

Você pode usar o seguinte script PHP para receber os arquivos enviados:

```php
<?php
// Verifica se foi enviado algum arquivo
if ($_FILES['file']['error'] === UPLOAD_ERR_OK) {
    $tmp_name = $_FILES['file']['tmp_name'];
    $name = basename($_FILES['file']['name']);
    
    // Move o arquivo para o diretório desejado
    move_uploaded_file($tmp_name, "/caminho/para/salvar/os/arquivos/$name");
    
    echo "Arquivo enviado com sucesso!";
} else {
    echo "Erro ao enviar arquivo.";
}
?>
```

Certifique-se de ajustar o caminho para o diretório onde deseja salvar os arquivos.


