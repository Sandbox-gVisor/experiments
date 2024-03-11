FROM gcc:latest
LABEL authors="AMats"

COPY . .

ENTRYPOINT ["./run_all.sh"]
