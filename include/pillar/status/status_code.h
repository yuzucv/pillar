#pragma once
// reference: https://github.com/abseil/abseil-cpp/blob/master/absl/status

#include <ostream>
#include <string>
#include <string_view>
#include <utility>

namespace yuzu
{
enum class StatusCode : int
{
    // returned on success.
    kOk = 0,
    // indicates the operation was cancelled.
    kCancelled = 1,
    // indicates an unknown error occurred.
    kUnknown = 2,
    // indicates the caller specified an invalid argument.
    kInvalidArgument = 3,
    // indicates a deadline expired before the operation could complete.
    kDeadlineExceeded = 4,
    // indicates some requested entity (such as a file or directory) was not found.
    kNotFound = 5,
    // indicates that the entity a caller attempted to create
    // (such as a file or directory) is already present.
    kAlreadyExists = 6,
    // indicates that the caller does not have permission to execute the specified operation.
    kPermissionDenied = 7,
    // indicates some resource has been exhausted, perhaps a per-user quota,
    // or perhaps the entire file system is out of space.
    kResourceExhausted = 8,
    // indicates that the operation was rejected because the system is
    // not in a state required for the operation's execution.
    kFailedPrecondition = 9,
    // indicates the operation was aborted.
    kAborted = 10,
    // indicates the operation was attempted past the valid range,
    // such as seeking or reading past an end-of-file.
    kOutOfRange = 11,
    // indicates the operation is not implemented or supported in this service.
    kUnimplemented = 12,
    // indicates an internal error has occurred and some invariants expected
    // by the underlying system have not been satisfied.
    // This error code is reserved for serious errors.
    kInternal = 13,
    // indicates the service is currently
    // unavailable and that this is most likely a transient condition.
    kUnavailable = 14,
    // indicates that unrecoverable data loss or corruption has occurred.
    kDataLoss = 15,
    // indicates that the request does not have valid authentication credentials
    // for the operation.
    kUnauthenticated = 16,
    // should not be used.
    kDoNotUseReservedForFutureExpansionUseDefaultInSwitchInstead_ = 20
};

/**
 * @brief Returns the name for the status code, or "" if it is an unknown value.
 *
 * @param code
 * @return std::string
 */
std::string StatusCodeToString(StatusCode code);

/**
 * @brief Convert canonical code to a value known to this binary.
 *
 * @param value
 * @return StatusCode
 */
StatusCode MapToLocalCode(int value);

/**
 * @brief Streams StatusCodeToString(code) to `os`.
 *
 * @param os
 * @param code
 * @return std::ostream&
 */
std::ostream& operator<<(std::ostream& os, StatusCode code);

class Status final
{
public:
    constexpr Status() : mCode(StatusCode::kUnknown) {}
    constexpr Status(StatusCode code) : mCode(code) {}
    constexpr Status(StatusCode code, std::string_view msg) : mCode(code), mMessage(msg) {}

    std::string toString() const;

    constexpr bool ok() const noexcept { return mCode == StatusCode::kOk; }
    constexpr StatusCode statusCode() const noexcept { return mCode; }
    constexpr auto rawCode() const noexcept { return static_cast<std::underlying_type<StatusCode>::type>(mCode); };
    constexpr std::string_view message() const { return mMessage; };

    friend bool operator==(const Status& lhs, const Status& rhs) { return lhs.rawCode() == rhs.rawCode(); }
    friend bool operator!=(const Status& lhs, const Status& rhs) { return !(lhs == rhs); }

private:
    StatusCode mCode;
    std::string_view mMessage;
};

Status okStatus();

std::ostream& operator<<(std::ostream& os, const Status& x);
} // namespace yuzu
