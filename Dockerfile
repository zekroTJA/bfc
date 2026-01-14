FROM docker.io/alpine:latest AS build
WORKDIR /build
COPY src/ src/
COPY Makefile .
RUN apk add --no-cache build-base
RUN make static

FROM scratch
COPY --from=build /build/dist/bfc /bfc
ENTRYPOINT [ "/bfc" ]
