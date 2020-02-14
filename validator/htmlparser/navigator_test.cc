//
// Copyright 2019 The AMP HTML Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS-IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the license.
//

#include "htmlparser/navigator.h"

#include <iostream>

#include "gtest/gtest.h"
#include "htmlparser/node.h"
#include "htmlparser/parser.h"

using namespace htmlparser;

TEST(NavigatorTest, BasicNavigation) {
  NodePtr doc = Parse("<html><head><noscript>No scripting</noscript></head>"
                      "<body><div>Hello world<div>foo bar</div></div>"
                      "<h1>foo baz</h1></body></html>");
  NodeNavigator navigator(doc);
  navigator.MoveToChild();  // <html>.
  EXPECT_EQ(navigator.Value(), "No scripting Hello world foo bar foo baz");
  EXPECT_EQ(navigator.Current()->DataAtom(), Atom::HTML);
  EXPECT_EQ(navigator.LocalName(), "html");

  navigator.MoveToChild();  // <head>
  EXPECT_EQ(navigator.Value(), "No scripting");
  EXPECT_EQ(navigator.Current()->DataAtom(), Atom::HEAD);
  EXPECT_EQ(navigator.LocalName(), "head");

  navigator.MoveToNext();  // <body>
  EXPECT_EQ(navigator.Value(), "Hello world foo bar foo baz");
  EXPECT_EQ(navigator.Current()->DataAtom(), Atom::BODY);
  EXPECT_EQ(navigator.LocalName(), "body");

  EXPECT_TRUE(navigator.MoveToChild());  // Body's first child div.
  EXPECT_EQ(navigator.Value(), "Hello world foo bar");  // two divs.
  EXPECT_EQ(navigator.Current()->DataAtom(), Atom::DIV);
  EXPECT_EQ(navigator.LocalName(), "div");

  EXPECT_TRUE(navigator.MoveToChild());
  EXPECT_EQ(navigator.Value(), "Hello world");  // Text node.
  // No tag name of text node.
  EXPECT_EQ(navigator.Current()->DataAtom(), Atom::UNKNOWN);
  EXPECT_EQ(navigator.LocalName(), "Hello world");

  // <div>foo bar</div> is a sibling of Hello world text node.
  EXPECT_TRUE(navigator.MoveToNext());
  EXPECT_EQ(navigator.Value(), "foo bar");  // Text node.
  EXPECT_EQ(navigator.Current()->DataAtom(), Atom::DIV);
  EXPECT_EQ(navigator.LocalName(), "div");

  // Go back to parent node.
  EXPECT_TRUE(navigator.MoveToParent());
  EXPECT_TRUE(navigator.MoveToNext());
  EXPECT_EQ(navigator.Value(), "foo baz");
  EXPECT_EQ(navigator.Current()->DataAtom(), Atom::H1);
  EXPECT_EQ(navigator.LocalName(), "h1");

  // Reset.
  navigator.MoveToRoot();
  NodeNavigator other = navigator.Clone();
  navigator.MoveToChild();  // <html>.
  EXPECT_EQ(navigator.Value(), "No scripting Hello world foo bar foo baz");
  EXPECT_EQ(navigator.Current()->DataAtom(), Atom::HTML);
}
