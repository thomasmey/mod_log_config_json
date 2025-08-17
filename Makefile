APXS=apxs

all: mod_log_config.so

mod_log_config.so: mod_log_config.c apr_json_escape.c
	$(APXS) -c mod_log_config.c apr_json_escape.c

install: mod_log_config.so
	$(APXS) -i -a -n log_config mod_log_config.la

clean:
	rm -f *.la *.lo *.slo .libs/*	
