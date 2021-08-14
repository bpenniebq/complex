#pragma once

#include "complex/Filter/ValueParameter.hpp"
#include "complex/complex_export.hpp"

namespace complex
{
class COMPLEX_EXPORT BoolParameter : public ValueParameter
{
public:
  using ValueType = bool;

  BoolParameter() = delete;
  BoolParameter(const std::string& name, const std::string& humanName, const std::string& helpText, ValueType defaultValue);
  ~BoolParameter() override = default;

  BoolParameter(const BoolParameter&) = delete;
  BoolParameter(BoolParameter&&) noexcept = delete;

  BoolParameter& operator=(const BoolParameter&) = delete;
  BoolParameter& operator=(BoolParameter&&) noexcept = delete;

  /**
   * @brief
   * @return
   */
  [[nodiscard]] Uuid uuid() const override;

  /**
   * @brief
   * @return
   */
  [[nodiscard]] AcceptedTypes acceptedTypes() const override;

  /**
   * @brief
   * @param value
   */
  [[nodiscard]] nlohmann::json toJson(const std::any& value) const override;

  /**
   * @brief
   * @return
   */
  [[nodiscard]] Result<std::any> fromJson(const nlohmann::json& json) const override;

  /**
   * @brief
   * @return
   */
  [[nodiscard]] UniquePointer clone() const override;

  /**
   * @brief
   * @return
   */
  [[nodiscard]] std::any defaultValue() const override;

  /**
   * @brief
   * @return
   */
  [[nodiscard]] ValueType defaultBool() const;

  /**
   * @brief
   * @param value
   * @return
   */
  [[nodiscard]] Result<> validate(const std::any& value) const override;

private:
  ValueType m_DefaultValue = {};
};
} // namespace complex