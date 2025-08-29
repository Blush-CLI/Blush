const http = require("http");
const { hostname } = require("os");
const path = require("path")
const fs = require('fs');

const PORT = 2063

const server = http.createServer((req, res) => {
    const urlpath = req.url

    if (urlpath == "/") {
        const filePath = path.join(__dirname, 'index.html');
        fs.readFile(filePath, (err, data) => {
            if (err) {
                res.writeHead(500);
                res.end('Error loading page');
                return;
            }
            res.writeHead(200, { 'Content-Type': 'text/html' });
            res.end(data);
        });
    } else if (urlpath == "/platforms") {
        const filePath = path.join(__dirname, 'platforms.html');
        fs.readFile(filePath, (err, data) => {
            if (err) {
                res.writeHead(500);
                res.end('Error loading page');
                return;
            }
            res.writeHead(200, { 'Content-Type': 'text/html' });
            res.end(data);
        });
    }
})

server.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}/`);
});