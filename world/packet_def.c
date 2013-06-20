/**
 * packet_def.c
 *
 * Defines the packet lookup tables
 */
#include <world/packet_def.h>

packet_def_t pkt_null = { .opcode = PKT_NULL, .type = PACKET_LEN_FIXED, .len = 0 };

packet_def_t packet_table_out[] = {
	{ .opcode = PKT_CL_UNKNOWN0, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN1, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN2, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN3, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN4, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN5, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN6, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN7, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN8, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN9, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN10, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN11, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN12, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN13, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN14, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN15, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN16, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN17, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN18, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN19, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN20, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN21, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN22, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN23, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN24, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_CL_UNKNOWN25, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN26, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN27, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN28, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN29, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN30, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN31, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN32, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN33, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN34, .type = PACKET_LEN_16, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN35, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN36, .type = PACKET_LEN_FIXED, .len = 3},
	{ .opcode = PKT_CL_UNKNOWN37, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN38, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN39, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN40, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN41, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN42, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN43, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN44, .type = PACKET_LEN_FIXED, .len = 5},
	{ .opcode = PKT_CL_UNKNOWN45, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN46, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN47, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN48, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN49, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN50, .type = PACKET_LEN_FIXED, .len = 9},
	{ .opcode = PKT_CL_UNKNOWN51, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN52, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN53, .type = PACKET_LEN_16, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN54, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN55, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN56, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN57, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN58, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN59, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN60, .type = PACKET_LEN_16, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN61, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_CL_UNKNOWN62, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN63, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN64, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN65, .type = PACKET_LEN_16, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN66, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN67, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN68, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN69, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN70, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN71, .type = PACKET_LEN_FIXED, .len = 3},
	{ .opcode = PKT_CL_UNKNOWN72, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_REGION_UPDATE, .type = PACKET_LEN_FIXED, .len = 4 },
	{ .opcode = PKT_CL_UNKNOWN74, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN75, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN76, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN77, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN78, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN79, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN80, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN81, .type = PACKET_LEN_16, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN82, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN83, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN84, .type = PACKET_LEN_FIXED, .len = 7},
	{ .opcode = PKT_CL_UNKNOWN85, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN86, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN87, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN88, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN89, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN90, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN91, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN92, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN93, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN94, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN95, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN96, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN97, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN98, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN99, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_CL_UNKNOWN100, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN101, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN102, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN103, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN104, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN105, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN106, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_CL_UNKNOWN107, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN108, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN109, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN110, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_CL_UNKNOWN111, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN112, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN113, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN114, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN115, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN116, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN117, .type = PACKET_LEN_FIXED, .len = 15},
	{ .opcode = PKT_CL_UNKNOWN118, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN119, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN120, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN121, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN122, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN123, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN124, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN125, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN126, .type = PACKET_LEN_16, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN127, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN128, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN129, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN130, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN131, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN132, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN133, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN134, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN135, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN136, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN137, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN138, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN139, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN140, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN141, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN142, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN143, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN144, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN145, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN146, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN147, .type = PACKET_LEN_FIXED, .len = 14},
	{ .opcode = PKT_CL_UNKNOWN148, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN149, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN150, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN151, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN152, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN153, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN154, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN155, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN156, .type = PACKET_LEN_FIXED, .len = 3},
	{ .opcode = PKT_CL_UNKNOWN157, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN158, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN159, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN160, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN161, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN162, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN163, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN164, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN165, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN166, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN167, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN168, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN169, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN170, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN171, .type = PACKET_LEN_FIXED, .len = 3},
	{ .opcode = PKT_CL_UNKNOWN172, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN173, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN174, .type = PACKET_LEN_FIXED, .len = 5},
	{ .opcode = PKT_CL_UNKNOWN175, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN176, .type = PACKET_LEN_FIXED, .len = 10},
	{ .opcode = PKT_CL_UNKNOWN177, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN178, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN179, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN180, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN181, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN182, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN183, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN184, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN185, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN186, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN187, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN188, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN189, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN190, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN191, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN192, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN193, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN194, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN195, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN196, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN197, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN198, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN199, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN200, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN201, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN202, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN203, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN204, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN205, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN206, .type = PACKET_LEN_FIXED, .len = 3},
	{ .opcode = PKT_CL_UNKNOWN207, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN208, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN209, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN210, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN211, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN212, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN213, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN214, .type = PACKET_LEN_16, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN215, .type = PACKET_LEN_FIXED, .len = 7},
	{ .opcode = PKT_CL_UNKNOWN216, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN217, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN218, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN219, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN220, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN221, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_CL_UNKNOWN222, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN223, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN224, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN225, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN226, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN227, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN228, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN229, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN230, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_CL_UNKNOWN231, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN232, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN233, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN234, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN235, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN236, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN237, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN238, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN239, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN240, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_CL_UNKNOWN241, .type = PACKET_LEN_16, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN242, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN243, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN244, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN245, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN246, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN247, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN248, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_CL_UNKNOWN249, .type = PACKET_LEN_FIXED, .len = 3},
	{ .opcode = PKT_CL_UNKNOWN250, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN251, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN252, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN253, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN254, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_CL_UNKNOWN255, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_CL_UNKNOWN256, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_NULL, .type = PACKET_LEN_FIXED, .len = 0 }
};

packet_def_t packet_table_in[] = {
	{ .opcode = PKT_SV_IDLE, .type = PACKET_LEN_FIXED, .len = 0 },
	{ .opcode = PKT_SV_UNKNOWN1, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN2, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_FOCUS_CHANGE, .type = PACKET_LEN_FIXED, .len = 1 },
	{ .opcode = PKT_SV_CHAT_MESSAGE, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN5, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN6, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN7, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN8, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN9, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN10, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN11, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN12, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN13, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN14, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN15, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN16, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN17, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN18, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN19, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN20, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN21, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN22, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN23, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN24, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN25, .type = PACKET_LEN_FIXED, .len = 12},
	{ .opcode = PKT_SV_UNKNOWN26, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN27, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN28, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN29, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN30, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN31, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN32, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN33, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN34, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN35, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN36, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_SV_UNKNOWN37, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN38, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN39, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN40, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN41, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN42, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN43, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN44, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN45, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN46, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN47, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN48, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN49, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN50, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN51, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN52, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN53, .type = PACKET_LEN_FIXED, .len = 12},
	{ .opcode = PKT_SV_UNKNOWN54, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN55, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN56, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN57, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN58, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN59, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN60, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN61, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN62, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN63, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN64, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN65, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN66, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN67, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN68, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN69, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN70, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN71, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN72, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN73, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN74, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN75, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN76, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN77, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN78, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN79, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN80, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN81, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN82, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN83, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN84, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN85, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_SV_UNKNOWN86, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_SV_UNKNOWN87, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN88, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN89, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN90, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN91, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN92, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN93, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN94, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN95, .type = PACKET_LEN_FIXED, .len = 3},
	{ .opcode = PKT_SV_UNKNOWN96, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN97, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN98, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN99, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN100, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN101, .type = PACKET_LEN_FIXED, .len = 13},
	{ .opcode = PKT_SV_UNKNOWN102, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN103, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN104, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN105, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN106, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN107, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN108, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN109, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN110, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN111, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN112, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN113, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN114, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN115, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN116, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN117, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN118, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN119, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN120, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_SV_UNKNOWN121, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN122, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN123, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN124, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN125, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN126, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN127, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN128, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN129, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN130, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN131, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_SV_UNKNOWN132, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN133, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN134, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN135, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN136, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN137, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN138, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN139, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN140, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN141, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN142, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN143, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN144, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN145, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN146, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN147, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN148, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN149, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN150, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN151, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN152, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_SV_UNKNOWN153, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN154, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN155, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN156, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN157, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN158, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN159, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN160, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN161, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN162, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN163, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN164, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN165, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN166, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN167, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN168, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN169, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN170, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN171, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN172, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN173, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN174, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN175, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN176, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN177, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN178, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN179, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN180, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN181, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN182, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN183, .type = PACKET_LEN_FIXED, .len = 3},
	{ .opcode = PKT_SV_UNKNOWN184, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN185, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN186, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN187, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN188, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN189, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_SV_UNKNOWN190, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN191, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN192, .type = PACKET_LEN_FIXED, .len = 12},
	{ .opcode = PKT_SV_UNKNOWN193, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN194, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN195, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN196, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN197, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN198, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN199, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN200, .type = PACKET_LEN_FIXED, .len = 2},
	{ .opcode = PKT_SV_UNKNOWN201, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN202, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN203, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN204, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN205, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN206, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN207, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN208, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_SV_UNKNOWN209, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN210, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_SV_UNKNOWN211, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN212, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN213, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN214, .type = PACKET_LEN_FIXED, .len = 7},
	{ .opcode = PKT_SV_UNKNOWN215, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN216, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN217, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN218, .type = PACKET_LEN_FIXED, .len = 10},
	{ .opcode = PKT_SV_UNKNOWN219, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN220, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN221, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN222, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN223, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN224, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN225, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN226, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN227, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN228, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN229, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN230, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_SV_UNKNOWN231, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN232, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN233, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN234, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN235, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN236, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN237, .type = PACKET_LEN_FIXED, .len = 8},
	{ .opcode = PKT_SV_UNKNOWN238, .type = PACKET_LEN_FIXED, .len = 1},
	{ .opcode = PKT_SV_UNKNOWN239, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN240, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_MOUSE_CLICK, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_SV_UNKNOWN242, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN243, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN244, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN245, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN246, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN247, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN248, .type = PACKET_LEN_8, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN249, .type = PACKET_LEN_FIXED, .len = 4},
	{ .opcode = PKT_SV_UNKNOWN250, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN251, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN252, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN253, .type = PACKET_LEN_FIXED, .len = 6},
	{ .opcode = PKT_SV_UNKNOWN254, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN255, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_SV_UNKNOWN256, .type = PACKET_LEN_FIXED, .len = 0},
	{ .opcode = PKT_NULL, .type = PACKET_LEN_FIXED, .len = 0 }
};

/**
 * packet_lookup
 *
 * Performs a lookup of a packet definition in either the server or
 * client packet tables.
 *  - type: One of PACKET_TYPE_{IN,OUT}
 *  - opcode: The opcode of the packet to lookup
 * returns: The packet definition, or a definition with opcode = PKT_NULL
 */
packet_def_t packet_lookup(int type, int opcode)
{
	packet_def_t* lookup_table;
	switch (type) {
	case PACKET_TYPE_IN:
		lookup_table = packet_table_in;
		break;
	case PACKET_TYPE_OUT:
		lookup_table = packet_table_out;
		break;
	default:
		return pkt_null;
	}
	for (int i = 0; ; i++) {
		if (lookup_table[i].opcode == opcode || lookup_table[i].opcode == PKT_NULL) {
			return lookup_table[i]; // we want to return even if it's the last entry (PKT_NULL)
		}
	}
	return pkt_null;
}
