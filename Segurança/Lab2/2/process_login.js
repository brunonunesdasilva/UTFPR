const express = require('express');
const session = require('express-session');
const bodyParser = require('body-parser');
const cookieParser = require('cookie-parser');

const app = express();
const PORT = 3000;

app.use(bodyParser.urlencoded({ extended: true }));
app.use(cookieParser());
app.use(
    session({
        secret: 'your_secret_key',
        resave: false,
        saveUninitialized: true,
        cookie: { maxAge: 7 * 24 * 60 * 60 * 1000 }
}));

app.get('/', (req, res) => {
    const { email, password } = req.session;
    const savedEmail = req.cookies.rememberEmail || '';
    const savedPassword = req.cookies.rememberPassword || '';
    res.send(`
        <form method="POST" action="/login">
            <input type="email" name="email" value="${savedEmail}" required>
            <input type="password" name="password"  value="${savedPassword}" required>
            <input type="submit" value="Login">
        </form>
        ${email && password ? `<p>Stored Session Data: Email - ${req.session.email}, Password - ${req.session.password}</p>` : ''}
    `);
});

app.post('/login', (req, res) => {
    const { email, password} = req.body;

    req.session.email = email;
    req.session.password = password;

    res.cookie('rememberEmail', email, { maxAge: 7 * 24 * 60 * 60 * 1000 }); // 1 semana
    res.cookie('rememberPassword', password, { maxAge: 7 * 24 * 60 * 60 * 1000 })

    res.send(`
        <h1>Login Successful</h1>
        <p>Email: ${email}</p>
        <p>Password: ${password}</p>
        <p>Session Data Stored</p>
        <a href="/">Go back</a>
    `);
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});