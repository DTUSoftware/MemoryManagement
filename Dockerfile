FROM gcc:latest
COPY . /MemoryManagement
WORKDIR /MemoryManagement/
RUN gcc -o mymem mymem.c
CMD ["./mymem"]