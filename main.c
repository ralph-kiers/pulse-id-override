#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pulse/proplist.h>

static char const* LIBPULSE_SO = "libpulse.so.0";

#define PROP_OVERRIDE_XLIST_ \
  PROP_OVERRIDE_X_(PULSE_APPLICATION_ID, application.id, id) \
  PROP_OVERRIDE_X_(PULSE_APPLICATION_NAME, application.name, name) \


typedef int (*pa_proplist_sets_func)(pa_proplist* p, char const* key,
    char const* value);

static pa_proplist_sets_func real_pa_proplist_sets;
static void* libpulse;

#define PROP_OVERRIDE_X_(ENV_, PROP_, VAR_) \
  static char const* VAR_;
PROP_OVERRIDE_XLIST_
#undef PROP_OVERRIDE_X_

__attribute__((constructor))
static void load(void) {
  libpulse = dlopen(LIBPULSE_SO, RTLD_NOW | RTLD_LOCAL);

  if (!libpulse) {
    fprintf(stderr, "pulse_id_hook: dlopen failed: %s\n", dlerror());
    goto fail0;
  }

  *((void**)(&real_pa_proplist_sets)) = dlsym(libpulse, "pa_proplist_sets");

  if (!real_pa_proplist_sets) {
    fprintf(stderr, "pulse_id_hook: dlsym failed: %s\n", dlerror());
    goto fail0;
  }

#define PROP_OVERRIDE_X_(ENV_, PROP_, VAR_) \
  VAR_ = getenv(#ENV_);
PROP_OVERRIDE_XLIST_
#undef PROP_OVERRIDE_X_

  return;

fail0:
  // The destructor will call dlclose, no need to also call it here.
  exit(1);
}

__attribute__((destructor))
static void unload(void) {
 if (libpulse) {
   dlclose(libpulse);
 }
}

static void override_prop_value(char const* restrict key,
    char const** restrict value) {
  if (!key) {
    return;
  }

#define PROP_OVERRIDE_X_(ENV_, PROP_, VAR_) \
  if (strcmp(key, #PROP_) == 0) { \
    if (VAR_) *value = VAR_; \
    return; \
  }
PROP_OVERRIDE_XLIST_
#undef PROP_OVERRIDE_X_
}

int pa_proplist_sets(pa_proplist* p, char const* key, char const* value) {
  override_prop_value(key, &value);
  return real_pa_proplist_sets(p, key, value);
}
