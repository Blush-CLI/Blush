const http = require("http");
const path = require("path");
const fs = require("fs");

const PORT = 2063;
const PUBLIC_DIR = path.join(__dirname, "public");

const server = http.createServer((req, res) => {
    const urlpath = req.url;

    if (urlpath === "/") {
        const filePath = path.join(PUBLIC_DIR, "index.html");
        fs.readFile(filePath, (err, data) => {
            if (err) {
                res.writeHead(500);
                res.end("Error loading page");
                return;
            }
            res.writeHead(200, { "Content-Type": "text/html" });
            res.end(data);
        });
    } else if (urlpath === "/platforms") {
        const filePath = path.join(PUBLIC_DIR, "platforms.html");
        fs.readFile(filePath, (err, data) => {
            if (err) {
                res.writeHead(500);
                res.end("Error loading page");
                return;
            }
            res.writeHead(200, { "Content-Type": "text/html" });
            res.end(data);
        });
    } else {
        const filePath = path.join(PUBLIC_DIR, urlpath);
        if (!filePath.startsWith(PUBLIC_DIR)) {
            res.writeHead(403, { "Content-Type": "text/plain" });
            res.end("403 Forbidden");
            return;
        }

        const ext = path.extname(filePath).toLowerCase();
        const mimeTypes = {
            ".html": "text/html",
            ".js": "application/javascript",
            ".css": "text/css",
            ".ico": "image/x-icon",
            ".png": "image/png",
            ".jpg": "image/jpeg",
            ".svg": "image/svg+xml",
            ".txt": "text/plain"
        };
        const contentType = mimeTypes[ext] || "application/octet-stream";

        fs.readFile(filePath, (err, data) => {
            if (err) {
                res.writeHead(404, { "Content-Type": "text/plain" });
                res.end("404 Not Found");
                return;
            }
            res.writeHead(200, { "Content-Type": contentType });
            res.end(data);
        });
    }
});

server.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}/`);
});
