const http = require("http");
const fs = require("fs");
const path = require("path");

const PORT = 2064;
const FILES_DIR = path.join(__dirname, "files");

const server = http.createServer((req, res) => {
    let safePath = path.normalize(req.url).replace(/^(\.\.[\/\\])+/, "");
    if (safePath === "/") safePath = "/index.html";
    const filePath = path.join(FILES_DIR, safePath);

    if (!fs.existsSync(FILES_DIR)) fs.mkdirSync(FILES_DIR);

    fs.stat(filePath, (err, stats) => {
        if (err || !stats.isFile()) {
            res.writeHead(404, { "Content-Type": "text/plain" });
            return res.end("File not found");
        }

        res.writeHead(200, {
            "Content-Disposition": `inline; filename="${path.basename(filePath)}"`,
            "Content-Length": stats.size,
            "Content-Type": "application/octet-stream",
        });

        const stream = fs.createReadStream(filePath);
        stream.pipe(res);

        stream.on("error", () => {
            if (!res.headersSent) {
                res.writeHead(500, { "Content-Type": "text/plain" });
            }
            res.end("Error reading file");
        });
    });
});

server.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}/`);
});
