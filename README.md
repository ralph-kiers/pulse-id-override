# pulse-id-override

Simple override of `application.id` and `application.name` for PulseAudio applications.

Not every application allows you to configure the aforementioned values. This can cause stream restoration behaviour for multiple running instances of the same application to interfere with each other. A well-known example of this is `vlc` using the PulseAudio backend on top of a PipeWire audio server.

Overriding the `application.id` and `application.name` using this library causes the stream restoration logic to be decoupled. Changing the values per instance allows you to still keep global stream restoration enabled while being able to split each instance separately as desired.

## Usage

Preload the compiled library using `LD_PRELOAD` and use the `PULSE_APPLICATION_ID` and `PULSE_APPLICATION_NAME` environment variables to override the `application.id` and `application.name` respectively.

## Examples

```
LD_PRELOAD=./pulse_id_override.so PULSE_APPLICATION_ID=foo PULSE_APPLICATION_NAME=bar vlc
```

## Build

Building is done through a simple `Makefile` with no support for out of tree builds. Simply run:
```
make
```
