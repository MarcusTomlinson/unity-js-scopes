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

#ifndef _UNITY_JS_PREVIEW_WIDGET_H_
#define _UNITY_JS_PREVIEW_WIDGET_H_

#include <unity/scopes/PreviewWidget.h>

#include <v8-cpp.h>

class PreviewWidget
{
 public:
  PreviewWidget(std::string const &id,
                std::string const &widget_type);
  PreviewWidget(const std::string& definition);
  ~PreviewWidget();

  // v8 binding
  void add_attribute_value(v8::FunctionCallbackInfo<v8::Value> const& args);
  void add_attribute_mapping(v8::FunctionCallbackInfo<v8::Value> const& args);
  void add_widget(v8::FunctionCallbackInfo<v8::Value> const& args);

  v8::Local<v8::Value> id(v8::FunctionCallbackInfo<v8::Value> const& args);
  v8::Local<v8::Value> widget_type(v8::FunctionCallbackInfo<v8::Value> const& args);

  v8::Local<v8::Value> attribute_mappings(v8::FunctionCallbackInfo<v8::Value> const& args);
  v8::Local<v8::Value> attribute_values(v8::FunctionCallbackInfo<v8::Value> const& args);
  v8::Local<v8::Value> widgets(v8::FunctionCallbackInfo<v8::Value> const& args);
  v8::Local<v8::Value> data(v8::FunctionCallbackInfo<v8::Value> const& args);

  const unity::scopes::PreviewWidget& get_preview_widget() const;

 private:
  unity::scopes::PreviewWidget preview_widget_;
};

#endif // _UNITY_JS_PREVIEW_WIDGET_H_