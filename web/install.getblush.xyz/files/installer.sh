#!/usr/bin/env bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

LOG_FILE="$HOME/blush_installation_latest.log"
MSG_OK="Blush installed and added to PATH!"
MSG_ABORT="Installation aborted!"
MSG_ARCH_FAIL="No binary available for your architecture: "

cleanup() {
    if [[ ! -z "$SPINNER_PID" ]]; then
        kill $SPINNER_PID 2>/dev/null
        echo -ne "\r"
    fi
    echo -e "\n${RED}$MSG_ABORT${NC}"
    exit 1
}
trap cleanup SIGINT

mkdir -p "$HOME/.local/bin"
INSTALL_PATH="$HOME/.local/bin"
if [[ "$PREFIX" == *"com.termux"* ]]; then
    INSTALL_PATH="$PREFIX/bin"
fi

ARCH=$(uname -m)
if [[ "$ARCH" == "x86_64" ]]; then
    BLUSH_ARCH="x64"
elif [[ "$ARCH" == "aarch64" ]]; then
    BLUSH_ARCH="arm64"
else
    echo -e "${RED}${MSG_ARCH_FAIL}$ARCH${NC}"
    exit 1
fi

URL="https://cdn.getblush.xyz/latest/blush-linux-$BLUSH_ARCH"
OUTPUT="$HOME/blush_tmp_download"

while true; do
    STATUS=$(curl -sI -w "%{http_code}" -o /dev/null "$URL")
    if [[ "$STATUS" == "429" ]]; then
        echo -e "${RED}Too Many Requests, retrying in 3s...${NC}"
        sleep 3
    else
        break
    fi
done

TOTAL=$(curl -sI "$URL" | grep -i '^Content-Length:' | awk '{print $2}' | tr -d '\r')
TOTAL=${TOTAL:-0}

rm -f "$OUTPUT"

curl -sS -o "$OUTPUT" "$URL" &
CURL_PID=$!

while kill -0 $CURL_PID 2>/dev/null; do
    if [[ $TOTAL -ne 0 ]]; then
        CURRENT=$(stat -c%s "$OUTPUT" 2>/dev/null || echo 0)
        PERCENT=$(( CURRENT * 100 / TOTAL ))
        [[ $PERCENT -gt 100 ]] && PERCENT=100
        echo -ne "\r${GREEN}[ ${PERCENT}% ] Downloading...${NC}"
    else
        echo -ne "\r${GREEN}[ ???% ] Downloading...${NC}"
    fi
    sleep 0.05
done

wait $CURL_PID
echo -e "\r${GREEN}[ 100% ] Downloading...${NC}\n${GREEN}Download finished!${NC}"

spinner() {
    local pid=$1
    local delay=0.1
    local spinstr='|/-\'
    echo -ne "${GREEN}Setting Up Blush ${NC}\n"
    while ps -p $pid &>/dev/null; do
        for i in $(seq 0 3); do
            echo -ne "\b${spinstr:$i:1}"
            sleep $delay
        done
    done
    echo -ne "\b"
}

chmod +x "$OUTPUT"
mv "$OUTPUT" "$INSTALL_PATH/blush" &
SPINNER_PID=$!
spinner $SPINNER_PID
wait $SPINNER_PID

echo -e "${GREEN}Finished!${NC}"
echo -e "${GREEN}$MSG_OK${NC}"
echo "$(date): Installed Blush at $INSTALL_PATH/blush" >> "$LOG_FILE"
