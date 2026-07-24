<?php
session_start();


if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST["email"] ?? '';
    $password = $_POST["password"] ?? '';


    $correct_username = "bruno@gmail.com";
    $correct_password = "123";

    if ($username == $correct_username && $password == $correct_password) {

        $_SESSION["email"] = $username;
        $_SESSION["password"] = $password;
        echo "<script>alert('Login realizado com sucesso');</script>";
    } else {
        echo "<script>alert('Usuário ou senha incorretos');</script>";
    }
}
?>

<!DOCTYPE html>
<html>

<head>
    <title>Login</title>
    <meta charset="UTF-8">
    <style>
        body {
            font-family: Arial, sans-serif;
            max-width: 400px;
            margin: 0 auto;
            padding: 20px;
        }

        form {
            border: 1px solid #ccc;
            padding: 20px;
            border-radius: 5px;
        }

        input[type="email"],
        input[type="password"] {
            width: 100%;
            padding: 8px;
            margin: 8px 0;
        }

        input[type="submit"] {
            background-color: #4CAF50;
            color: white;
            padding: 10px;
            border: none;
            cursor: pointer;
        }

        input[type="submit"]:hover {
            background-color: #45a049;
        }
    </style>
</head>

<body>
    <h1>Login</h1>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <label for="email">Email do usuário:</label>
        <input type="email" id="email" name="email" required><br><br>

        <label for="password">Senha:</label>
        <input type="password" id="password" name="password" required><br><br>

        <input type="submit" value="Entrar">
    </form>
</body>

</html>