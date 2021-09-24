#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include <queue>
#include <utility>

namespace esphome {
namespace text_sensor {

class TextSensor;

/** Apply a filter to text sensor values such as toUpper.
 *
 * This class is purposefully kept quite simple, since more complicated
 * filters should really be done with the filter sensor in Home Assistant.
 */
class Filter {
 public:
  /** This will be called every time the filter receives a new value.
   *
   * It can return an empty optional to indicate that the filter chain
   * should stop, otherwise the value in the filter will be passed down
   * the chain.
   *
   * @param value The new value.
   * @return An optional string, the new value that should be pushed out.
   */
  virtual optional<std::string> new_value(std::string value);

  /// Initialize this filter, please note this can be called more than once.
  virtual void initialize(TextSensor *parent, Filter *next);

  void input(std::string value);

  void output(std::string value);

 protected:
  friend TextSensor;

  Filter *next_{nullptr};
  TextSensor *parent_{nullptr};
};

using lambda_filter_t = std::function<optional<std::string>(std::string)>;

/** This class allows for creation of simple template filters.
 *
 * The constructor accepts a lambda of the form float -> optional<float>.
 * It will be called with each new value in the filter chain and returns the modified
 * value that shall be passed down the filter chain. Returning an empty Optional
 * means that the value shall be discarded.
 */
class LambdaFilter : public Filter {
 public:
  explicit LambdaFilter(lambda_filter_t lambda_filter);

  optional<std::string> new_value(std::string value) override;

  const lambda_filter_t &get_lambda_filter() const;
  void set_lambda_filter(const lambda_filter_t &lambda_filter);

 protected:
  lambda_filter_t lambda_filter_;
};

/// A simple filter that converts all text to uppercase
class ToUpperFilter : public Filter {
 public:
  explicit ToUpperFilter(std::string dummy);
  // explicit ToUpperFilter();

  optional<std::string> new_value(std::string value) override;

 protected:
};

}  // namespace sensor
}  // namespace esphome
