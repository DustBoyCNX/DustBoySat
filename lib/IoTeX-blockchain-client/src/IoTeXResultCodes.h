#ifndef IOTEX_RESULT_CODES_H
#define IOTEX_RESULT_CODES_H

namespace iotex
{
enum class ResultCode
{
	SUCCESS = 0,
	ERROR_HTTP,
	ERROR_JSON_PARSE,
	ERROR_BAD_PARAMETER,
	ERROR_SIGNATURE,
	ERROR_UNKNOWN,
	ERROR_STORAGE_OPEN,
	ERROR_STORAGE_BAD_READ,
	ERROR_STORAGE_EMPTY,
	ERROR_GRPC,
	ERROR_WRONG_TYPE,
	VALUES_COUNT
};
} // namespace iotex

#endif