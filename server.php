<?php
// Verifica se foi enviado algum arquivo
if ($_FILES['file']['error'] === UPLOAD_ERR_OK) {
    $tmp_name = $_FILES['file']['tmp_name'];
    $name = basename($_FILES['file']['name']);
    
    // Move o arquivo para o diretório desejado
    move_uploaded_file($tmp_name, "home/$name");
    
    echo "Arquivo enviado com sucesso!";
} else {
    echo "Erro ao enviar arquivo.";
}
?>
