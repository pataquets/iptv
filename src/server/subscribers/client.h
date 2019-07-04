/*  Copyright (C) 2014-2019 FastoGT. All right reserved.
    This file is part of iptv_cloud.
    iptv_cloud is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    iptv_cloud is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with iptv_cloud.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <common/net/types.h>

#include <fastotv/client.h>
#include <fastotv/commands_info/channels_info.h>
#include <fastotv/commands_info/ping_info.h>
#include <fastotv/commands_info/runtime_channel_info.h>
#include <fastotv/protocol/types.h>

#include "server/subscribers/server_auth_info.h"

namespace iptv_cloud {
namespace server {
namespace subscribers {

class SubscriberClient : public fastotv::Client {
 public:
  typedef ServerAuthInfo host_info_t;
  typedef fastotv::Client base_class;

  SubscriberClient(common::libev::IoLoop* server, const common::net::socket_info& info);

  const char* ClassName() const override;

  void SetServerHostInfo(const host_info_t& info);
  host_info_t GetServerHostInfo() const;

  void SetCurrentStreamID(fastotv::stream_id sid);
  fastotv::stream_id GetCurrentStreamID() const;

 private:
  host_info_t hinfo_;
  fastotv::stream_id current_stream_id_;
};

class ProtocoledSubscriberClient : public fastotv::protocol::ProtocolClient<SubscriberClient> {
 public:
  typedef fastotv::protocol::ProtocolClient<SubscriberClient> base_class;
  ProtocoledSubscriberClient(common::libev::IoLoop* server, const common::net::socket_info& info);

  // requests
  common::ErrnoError Ping() WARN_UNUSED_RESULT;
  common::ErrnoError Ping(const fastotv::commands_info::ClientPingInfo& ping) WARN_UNUSED_RESULT;

  // responces
  common::ErrnoError CheckActivateFail(fastotv::protocol::sequance_id_t id, common::Error err) WARN_UNUSED_RESULT;

  common::ErrnoError ActivateFail(fastotv::protocol::sequance_id_t id, common::Error err) WARN_UNUSED_RESULT;
  common::ErrnoError ActivateSuccess(fastotv::protocol::sequance_id_t id) WARN_UNUSED_RESULT;

  common::ErrnoError Pong(fastotv::protocol::sequance_id_t id) WARN_UNUSED_RESULT;
  common::ErrnoError Pong(fastotv::protocol::sequance_id_t id,
                          const fastotv::commands_info::ServerPingInfo& pong) WARN_UNUSED_RESULT;

  common::ErrnoError GetServerInfoFail(fastotv::protocol::sequance_id_t id, common::Error err) WARN_UNUSED_RESULT;
  common::ErrnoError GetServerInfoSuccess(fastotv::protocol::sequance_id_t id,
                                          const common::net::HostAndPort& bandwidth_host) WARN_UNUSED_RESULT;

  common::ErrnoError GetChannelsFail(fastotv::protocol::sequance_id_t id, common::Error err) WARN_UNUSED_RESULT;
  common::ErrnoError GetChannelsSuccess(fastotv::protocol::sequance_id_t id,
                                        const fastotv::commands_info::ChannelsInfo& channels) WARN_UNUSED_RESULT;

  common::ErrnoError GetRuntimeChannelInfoSuccess(fastotv::protocol::sequance_id_t id,
                                                  fastotv::stream_id sid,
                                                  size_t watchers) WARN_UNUSED_RESULT;
  common::ErrnoError GetRuntimeChannelInfoSuccess(fastotv::protocol::sequance_id_t id,
                                                  const fastotv::commands_info::RuntimeChannelInfo& channel)
      WARN_UNUSED_RESULT;

 private:
  fastotv::protocol::sequance_id_t NextRequestID();

  std::atomic<fastotv::protocol::seq_id_t> id_;
};

}  // namespace subscribers
}  // namespace server
}  // namespace iptv_cloud
