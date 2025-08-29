const http = require("http");
const fs = require("fs");

const PORT = 2064;

const server = http.createServer((req, res) => {
    const urlpath = req.url;

    if (urlpath.startsWith("/get")) {
        const fullUrl = new URL(req.url, `http://${req.headers.host}`);
        const file = fullUrl.searchParams.get("file");

        if (!file) {
            res.writeHead(400, { "Content-Type": "text/plain" });
            return res.end("Missing file parameter");
        }

        if (!fs.existsSync("./files")) {
            fs.mkdirSync("./files");
        }

        const filePath = `./files/${file}`;

        if (fs.existsSync(filePath)) {
            const stat = fs.statSync(filePath);
            res.writeHead(200, {
                "Content-Disposition": `attachment; filename="${file}"`,
                "Content-Length": stat.size,
                "Content-Type": "application/octet-stream"
            });
            const fileStream = fs.createReadStream(filePath);
            fileStream.pipe(res);
            fileStream.on("error", () => {
                if (!res.headersSent) {
                    res.writeHead(500, { "Content-Type": "text/plain" });
                }
                res.end("Error reading file");
            });
        } else {
            res.writeHead(404, { "Content-Type": "text/plain" });
            res.end("File not found");
        }
    } else {
        res.writeHead(404, { "Content-Type": "text/plain" });
        res.end("Not Found");
    }
});

server.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}/`);
});
