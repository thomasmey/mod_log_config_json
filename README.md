# Overview

This repository provides a drop-in replacement for the Apache HTTP Server’s mod_log_config module.
It is based on the Apache HTTPD 2.4.x branch and extends the original functionality by adding native JSON output support for access logs.

The module is distributed as a shared object (mod_log_config.so) that can be dynamically loaded by httpd using the LoadModule directive.
Users can replace their existing mod_log_config.so with the binary built from this repository without changing the rest of their Apache installation.

## Motivation

Traditional Apache access logs are written in plain text with a configurable format. While flexible for humans, plain text logs are inconvenient for modern log ingestion pipelines.

This fork introduces structured JSON log output, which is particularly useful when integrating Apache HTTPD logs into log processors and observability systems such as:

- Fluent Bit
- Fluentd
- Elasticsearch, OpenSearch, and other JSON-based log storage backends
- Centralized monitoring and alerting platforms

By emitting JSON directly from Apache, the need for additional parsing and format conversion is eliminated. This results in:
- Faster ingestion
- Fewer parsing errors
- Cleaner and more consistent structured logs
- Seamless integration with existing JSON-based log pipelines

## Features

- Fully compatible with Apache HTTPD 2.4.x
- Supports all existing mod_log_config features
- Adds a new json log formatter that outputs log entries as valid JSON objects (RFC 8259 compliant)
- Optional short mode for compact JSON keys
- Drop-in replacement: no changes needed beyond swapping the module

## Build & Installation
### Prerequisites

- Apache HTTP Server 2.4.x (development headers required)
- Standard build tools (make, gcc, etc.)

### Build

To build the module, simply run:

´´´sh
make
´´´

### Install

To install into your Apache module directory:

´´´sh
sudo make install
´´´

This will place the new mod_log_config.so into the correct Apache module path (overwriting the existing one, so back up your original if needed).
Alternatively, you can manually copy the built .so to your Apache module directory and update httpd.conf accordingly.

## Usage

The new JSON formatter can be enabled using the CustomLog directive.

### Examples

Standard JSON output

## Compatibility

Based on Apache HTTPD 2.4.x branch

Intended as a drop-in replacement for the stock mod_log_config

Other versions are not officially supported

## License

This project follows the same license as Apache HTTPD:
Apache License, Version 2.0.
See the LICENSE file or https://www.apache.org/licenses/LICENSE-2.0 for details.
