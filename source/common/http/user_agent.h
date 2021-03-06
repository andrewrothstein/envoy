#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "envoy/http/header_map.h"
#include "envoy/stats/stats_macros.h"

namespace Envoy {
/**
 * All stats for user agents. @see stats_macros.h
 */
// clang-format off
#define ALL_USER_AGENTS_STATS(COUNTER)                                                             \
  COUNTER(downstream_cx_total)                                                                     \
  COUNTER(downstream_cx_destroy_remote_active_rq)                                                  \
  COUNTER(downstream_rq_total)
// clang-format on

/**
 * Wrapper struct for user agent stats. @see stats_macros.h
 */
struct UserAgentStats {
  ALL_USER_AGENTS_STATS(GENERATE_COUNTER_STRUCT)
};

namespace Http {

/**
 * Stats support for specific user agents.
 */
class UserAgent {
public:
  /**
   * Complete a connection length timespan for the target user agent.
   * @param span supplies the timespan to complete.
   */
  void completeConnectionLength(Stats::Timespan& span);

  /**
   * Initialize the user agent from request headers. This is only done once and the user-agent
   * is assumed to be the same for further requests.
   * @param headers supplies the request headers.
   * @param prefix supplies the stat prefix for the UA stats.
   * @param stat_store supplies the backing stat store.
   */
  void initializeFromHeaders(const HeaderMap& headers, const std::string& prefix,
                             Stats::Store& stat_store);

  /**
   * Called when a connection is being destroyed.
   * @param events supplies the network events that caused destruction.
   * @param active_streams supplies whether there are still active streams at the time of closing.
   */
  void onConnectionDestroy(uint32_t events, bool active_streams);

private:
  enum class Type { NotInitialized, iOS, Android, Unknown };

  Type type_{Type::NotInitialized};
  std::unique_ptr<UserAgentStats> stats_;
  std::string prefix_;
};

} // Http
} // Envoy
