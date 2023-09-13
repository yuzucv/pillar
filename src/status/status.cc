#include <sstream>
#include <string_view>
#include <strstream>

#include "pillar/status/status_code.h"

namespace yuzu
{
std::string StatusCodeToString(StatusCode code)
{
    switch (code)
    {
        case StatusCode::kOk:
            return "OK";
        case StatusCode::kCancelled:
            return "CANCELLED";
        case StatusCode::kUnknown:
            return "UNKNOWN";
        case StatusCode::kInvalidArgument:
            return "INVALID_ARGUMENT";
        case StatusCode::kDeadlineExceeded:
            return "DEADLINE_EXCEEDED";
        case StatusCode::kNotFound:
            return "NOT_FOUND";
        case StatusCode::kAlreadyExists:
            return "ALREADY_EXISTS";
        case StatusCode::kPermissionDenied:
            return "PERMISSION_DENIED";
        case StatusCode::kResourceExhausted:
            return "RESOURCE_EXHAUSTED";
        case StatusCode::kFailedPrecondition:
            return "FAILED_PRECONDITION";
        case StatusCode::kAborted:
            return "ABORTED";
        case StatusCode::kOutOfRange:
            return "OUT_OF_RANGE";
        case StatusCode::kUnimplemented:
            return "UNIMPLEMENTED";
        case StatusCode::kInternal:
            return "INTERNAL";
        case StatusCode::kUnavailable:
            return "UNAVAILABLE";
        case StatusCode::kDataLoss:
            return "DATA_LOSS";
        case StatusCode::kUnauthenticated:
            return "UNAUTHENTICATED";
        default:
            return "";
    }
}

StatusCode MapToLocalCode(int value)
{
    StatusCode code = static_cast<StatusCode>(value);
    switch (code)
    {
        case StatusCode::kOk:
        case StatusCode::kCancelled:
        case StatusCode::kUnknown:
        case StatusCode::kInvalidArgument:
        case StatusCode::kDeadlineExceeded:
        case StatusCode::kNotFound:
        case StatusCode::kAlreadyExists:
        case StatusCode::kPermissionDenied:
        case StatusCode::kResourceExhausted:
        case StatusCode::kFailedPrecondition:
        case StatusCode::kAborted:
        case StatusCode::kOutOfRange:
        case StatusCode::kUnimplemented:
        case StatusCode::kInternal:
        case StatusCode::kUnavailable:
        case StatusCode::kDataLoss:
        case StatusCode::kUnauthenticated:
            return code;
        default:
            return StatusCode::kUnknown;
    }
}

std::string Status::toString() const
{
    std::stringstream ss;
    ss << StatusCodeToString(statusCode());
    std::string_view msg = message();
    if (!msg.empty())
    {
        ss << ": " << msg;
    }

    return ss.str();
}

std::ostream& operator<<(std::ostream& os, StatusCode code) { return os << StatusCodeToString(code); }
std::ostream& operator<<(std::ostream& os, const Status& x) { return os << x.toString(); }

Status okStatus() { return Status(StatusCode::kOk); }
} // namespace yuzu
