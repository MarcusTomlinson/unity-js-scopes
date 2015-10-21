/*
 * Copyright 2015 Canonical Ltd.
 *
 * This file is part of unity-js-scopes.
 *
 * unity-js-scopes is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * unity-js-scopes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "registry.h"

#include "event_queue.h"

namespace {

std::map<std::shared_ptr<Registry>,
         core::ScopedConnection> active_scope_state_connections_;
std::map<std::shared_ptr<Registry>,
         core::ScopedConnection> active_list_update_connections_;

}

Registry::Registry(unity::scopes::RegistryProxy proxy)
  : isolate_(v8::Isolate::GetCurrent())
  , proxy_(proxy) {
}

unity::scopes::ScopeMetadata Registry::get_metadata(std::string const &scope_id) {
  return proxy_->get_metadata(scope_id);
}

unity::scopes::MetadataMap Registry::list() {
  return proxy_->list();
}

unity::scopes::MetadataMap Registry::list_if(v8::Local<v8::Function> predicate) {
  return proxy_->list_if([this, predicate] (unity::scopes::ScopeMetadata const& item) -> bool {
      v8::Handle<v8::Value> result =
        v8cpp::call_v8_with_receiver(
          isolate_,
          v8cpp::to_v8(isolate_, shared_from_this()),
          predicate,
          v8cpp::to_v8(isolate_, item)
      );
      return v8cpp::from_v8<bool>(isolate_, result);
    });
}

bool Registry::is_scope_running(const std::string& scope_id) {
  return proxy_->is_scope_running(scope_id);
}

void Registry::set_scope_state_callback(const std::string& scope_id,
                                        v8::Local<v8::Function> callback) {
  {
    auto it = active_scope_state_connections_.find(shared_from_this());
    if (it != active_scope_state_connections_.end()) {
      active_scope_state_connections_.erase(it);
    }
  }

  core::ScopedConnection connection =
    proxy_->set_scope_state_callback(scope_id, [this, callback] (bool is_running) {
        EventQueue::instance().run(isolate_, [this, callback, is_running] {
            v8cpp::call_v8_with_receiver(
                isolate_,
                v8cpp::to_v8(isolate_, shared_from_this()),
                callback,
                v8cpp::to_v8(isolate_, is_running)
            );
          });
    });

  active_scope_state_connections_.insert(
      std::make_pair(
          shared_from_this(),
          core::ScopedConnection(std::move(connection))));
}

void Registry::set_list_update_callback(v8::Local<v8::Function> callback) {
  {
    auto it = active_list_update_connections_.find(shared_from_this());
    if (it != active_list_update_connections_.end()) {
      active_list_update_connections_.erase(it);
    }
  }

  core::ScopedConnection connection =
    proxy_->set_list_update_callback([this, callback] () {
        EventQueue::instance().run(isolate_, [this, callback] {
            v8cpp::call_v8_with_receiver(
                isolate_,
                v8cpp::to_v8(isolate_, shared_from_this()),
                callback
            );
          });
    });

  active_list_update_connections_.insert(
      std::make_pair(
          shared_from_this(),
          core::ScopedConnection(std::move(connection))));
}

