#include "zfos.h"

#include "zf_driver.h"
#include "zf_device.h"

#include "board.h"
struct zfos_t zfos;

/**
 * @brief
 *
 * @param arg_zfos
 * @return char
 */
char zfos_init(struct zfos_t *arg_zfos)
{
	memcpy(&zfos, arg_zfos, sizeof(struct zfos_t));
	zfos.os_version = 1;
	printf("\r\n*** zfos %d ***\r\n", zfos.os_version);
	printf("platform:%d  %d\r\n", zfos.platform, zfos.series);
	printf("board_name:%s %d\r\n", zfos.board_name, zfos.board_version);

	board_init();

	zf_driver_init_hw();

	zf_driver_common_init();

	zf_device_init();
	printf("begin\r\n");
	// init fs
	zf_fs_init();
	return 0;
}
