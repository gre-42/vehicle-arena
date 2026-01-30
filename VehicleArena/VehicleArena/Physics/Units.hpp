// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cmath>

namespace VA {

static const float milli = 1.f / 1000.f;
static const float centi = 1.f / 100.f;
static const float percent = 1.f / 100.f;
static const float kilo = 1000.f;

static const float seconds = float{1e3};
static const float meters = 1.f;
static const float kg = float(1e-3);

static const float radians = 1.f;
static const float degrees = float(M_PI / 180);

static const float minutes = 60.f * seconds;
static const float hours = 60.f * minutes;

static const float mm = milli * meters;
static const float cm = centi * meters;
static const float km = kilo * meters;

static const float kph = km / hours;
static const float N = kg * meters / (seconds * seconds);
static const float W = N * meters / seconds;
static const float hp = 735.5f * W;
static const float rpm = float(2 * M_PI) / minutes;
static const float rps = float(2 * M_PI) / seconds;
static const float J = N * meters;

static const float Hz = 1 / seconds;

}
