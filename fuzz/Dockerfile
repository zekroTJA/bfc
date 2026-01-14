FROM aflplusplus/aflplusplus
WORKDIR /src
COPY fuzz/entrypoint.sh /bin/entrypoint.sh
COPY src/ src/
COPY fuzz/in/ inputs/
COPY Makefile .
VOLUME /output
RUN make CC=afl-cc
ENV AFL_EXIT_WHEN_DONE=true
ENTRYPOINT ["/bin/entrypoint.sh"]
