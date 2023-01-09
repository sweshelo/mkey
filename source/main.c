/*
 * mkey - parental controls master key generator for certain video game consoles
 * Copyright (C) 2015-2019, Daz Jones (Dazzozo) <daz@dazzozo.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mkey.h"
#include "types.h"

#define MAIN_ARGS_REQUIRED (2)

const char* main_format_devices(void);
void main_set_data_path(mkey_ctx* ctx);

int main(int argc, const char* argv[])
{
  mkey_ctx ctx;
  mkey_init(&ctx, false, NULL);

  main_set_data_path(&ctx);

  const char* default_device = "CTR"; //teh bias
  mkey_set_default_device(&ctx, default_device); 

  const char* aux = NULL;

  const char* device = "CTR";
  bool debug = false;
  char master_key[10] = {0};

  u8 month = strtoul(argv[2], 0, 10);
  u8 day = strtoul(argv[3], 0, 10);

  char inq_3ds[20];
  strcpy(inq_3ds, argv[1]);

  if(strlen(inq_3ds) != 10 && strlen(inq_3ds) != 8) {
    printf("Error: inquiry number must be 8 or 10 number digits.\n");
    exit(EXIT_FAILURE);
  }

  mkey_generate(&ctx, inq_3ds, month, day, aux, device, master_key);
  printf("%s\n", master_key);

  return 0;
}

const char* main_format_devices(void)
{
  size_t size = (mkey_num_devices() * (sizeof(*mkey_devices) + 2)) + 1;
  char* buffer = malloc(size);
  memset(buffer, 0, size);

  strcat(buffer, "{");

  for(int i = 0; i < mkey_num_devices(); i++) {
    strcat(buffer, mkey_devices[i]);
    if(i != mkey_num_devices() - 1) strcat(buffer, ", ");
  }

  strcat(buffer, "}");
  return buffer;
}

#if __linux__
#include <unistd.h>
#elif _WIN32
#include <Windows.h>
#endif

// :-(
void main_set_data_path(mkey_ctx* ctx)
{
  mkey_set_data_path(ctx,"data");
}
