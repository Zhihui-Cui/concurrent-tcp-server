#!/usr/bin/env python3
"""One manual test per server launch. Run from a second Ubuntu terminal."""

import argparse
import socket
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "case",
        choices=("normal", "fragmented", "incomplete", "max", "oversize"),
    )
    args = parser.parse_args()

    if args.case in ("normal", "fragmented"):
        payload = b"hello\n"
        expected = payload
    elif args.case == "max":
        payload = b"a" * 1023 + b"\n"
        expected = payload
    elif args.case == "incomplete":
        payload = b"hel"
        expected = b""
    else:
        payload = b"a" * 1024
        expected = b""

    with socket.create_connection(("127.0.0.1", 8080), timeout=3) as client:
        if args.case == "fragmented":
            client.sendall(payload[:3])
            time.sleep(1)
            client.sendall(payload[3:])
        else:
            client.sendall(payload)

        # Only the incomplete case needs EOF to make the server reject it.
        # The oversize case must be rejected without waiting for EOF.
        if args.case == "incomplete":
            client.shutdown(socket.SHUT_WR)

        response = bytearray()
        while True:
            chunk = client.recv(1024)
            if not chunk:
                break
            response.extend(chunk)
            if len(response) > len(expected):
                raise AssertionError("Server returned more bytes than expected")

    if bytes(response) != expected:
        raise AssertionError(
            f"Unexpected response: expected {expected!r}, got {bytes(response)!r}"
        )
    print(f"PASS {args.case}: sent {len(payload)} bytes, received {len(response)} bytes")


if __name__ == "__main__":
    main()
