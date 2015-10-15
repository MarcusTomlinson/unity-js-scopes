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

#ifndef _UNITY_JS_PREVIEW_QUERY_H_
#define _UNITY_JS_PREVIEW_QUERY_H_

#include <unity/scopes/ActionMetadata.h>
#include <unity/scopes/PreviewQueryBase.h>

#include "action-metadata.h"
#include "result.h"

#include <v8-cpp.h>

/**

--doc:class PreviewQuery
 * Represents a particular preview
 * 
 * @class PreviewQuery
 * 
 * @example

     var scopes = require('unity-js-scopes')
     function on_preview(result, action_metadata) {
       return new scopes.lib.preview_query(
         result,
         action_metadata,
         // run
         function(preview_reply) {},
         // cancelled
         function() {});
     }
     scopes.self.initialize(
      {}
      ,
      {
        run: function() { },
        start: function(scope_id) { },
        preview: on_preview,
      }
    );

--/doc:class

--doc:prototype PreviewQuery
--doc:member
 * Get result for this preview request
 * @return Result
--doc:/member
result: function() {
}
--/doc:member

--doc:member
 * Get metadata for this preview request
 * @return ActionMetadata
--doc:/member
action_metadata: function() {
}
--/doc:member

--doc:member
 * Check whether this query is still valid
 * @return Boolean
--doc:/member
valid: function() {
}
--/doc:member

--doc:member
 * Returns a dictionary with the scope's current settings
 * @return Dictionary
--doc:/member
settings: function() {
}
--/doc:member

--/doc:prototype

 */

class PreviewQuery : public unity::scopes::PreviewQueryBase,
    public std::enable_shared_from_this<PreviewQuery>
{
 public:
  PreviewQuery(unity::scopes::Result const& result,
               unity::scopes::ActionMetadata const& metadata,
               const v8::Local<v8::Function> &run_callback,
               const v8::Local<v8::Function> &cancelled_callback);
  ~PreviewQuery() override;

  // PreviewQueryBase implementation
  void run(unity::scopes::PreviewReplyProxy const& reply);

  // QueryBase implementation
  void cancelled() override;

  // v8 bindings
  std::shared_ptr<ActionMetaData> action_metadata() const;
  std::shared_ptr<Result> result() const;
  
 private:
  v8::Isolate* isolate_;

  v8::Persistent<v8::Function> run_callback_;
  v8::Persistent<v8::Function> cancelled_callback_;
};

#endif // _UNITY_JS_PREVIEW_QUERY_H_
