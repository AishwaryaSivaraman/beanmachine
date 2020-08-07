// Copyright (c) Facebook, Inc. and its affiliates.
#include <cmath>

#include "beanmachine/graph/operator/unaryop.h"
#include "beanmachine/graph/util.h"

namespace beanmachine {
namespace oper {

void complement(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  if (parent.type == graph::AtomicType::BOOLEAN) {
    node->value._bool = !parent._bool;
  } else if (parent.type == graph::AtomicType::PROBABILITY) {
    node->value._double = 1 - parent._double;
  } else {
    throw std::runtime_error(
        "invalid parent type " + parent.type.to_string() +
        " for COMPLEMENT operator at node_id " + std::to_string(node->index));
  }
}

void to_real(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  if (parent.type == graph::AtomicType::BOOLEAN) {
    node->value._double = parent._bool ? 1.0 : 0.0;
  } else if (
      parent.type == graph::AtomicType::REAL or
      parent.type == graph::AtomicType::POS_REAL or
      parent.type == graph::AtomicType::PROBABILITY) {
    node->value._double = parent._double;
  } else if (parent.type == graph::AtomicType::NATURAL) {
    node->value._double = (double)parent._natural;
  } else {
    throw std::runtime_error(
        "invalid parent type " + parent.type.to_string() +
        " for TO_REAL operator at node_id " + std::to_string(node->index));
  }
}

void to_pos_real(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  if (parent.type == graph::AtomicType::BOOLEAN) {
    node->value._double = parent._bool ? 1.0 : 0.0;
  } else if (
      parent.type == graph::AtomicType::POS_REAL or
      parent.type == graph::AtomicType::PROBABILITY) {
    node->value._double = parent._double;
  } else if (parent.type == graph::AtomicType::NATURAL) {
    node->value._double = (double)parent._natural;
  } else {
    throw std::runtime_error(
        "invalid parent type " + parent.type.to_string() +
        " for TO_POS_REAL operator at node_id " + std::to_string(node->index));
  }
}

void negate(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  if (parent.type == graph::AtomicType::REAL) {
    node->value._double = -parent._double;
  } else {
    throw std::runtime_error(
        "invalid parent type " + parent.type.to_string() +
        " for NEGATE operator at node_id " + std::to_string(node->index));
  }
}

void exp(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  if (parent.type == graph::AtomicType::REAL or
      parent.type == graph::AtomicType::POS_REAL) {
    node->value._double = std::exp(parent._double);
  } else {
    throw std::runtime_error(
        "invalid parent type " + parent.type.to_string() +
        " for EXP operator at node_id " + std::to_string(node->index));
  }
}

void expm1(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  if (parent.type == graph::AtomicType::REAL or
      parent.type == graph::AtomicType::POS_REAL) {
    node->value._double = std::expm1(parent._double);
  } else {
    throw std::runtime_error(
        "invalid parent type " + parent.type.to_string() +
        " for EXPM1 operator at node_id " + std::to_string(node->index));
  }
}

void phi(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  assert(parent.type == graph::AtomicType::REAL);
  // note: we have to cast it to an AtomicValue object rather than directly
  // assigning to ensure that the usual boundary checks for probabilities
  // are made
  node->value = graph::AtomicValue(
      graph::AtomicType::PROBABILITY, util::Phi(parent._double));
}

void logistic(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  assert(parent.type == graph::AtomicType::REAL);
  // note: we have to cast it to an AtomicValue object rather than directly
  // assigning to ensure that the usual boundary checks for probabilities
  // are made
  node->value = graph::AtomicValue(
      graph::AtomicType::PROBABILITY, util::logistic(parent._double));
}

void log1pexp(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  if (parent.type == graph::AtomicType::REAL or
      parent.type == graph::AtomicType::POS_REAL) {
    node->value._double = util::log1pexp(parent._double);
  } else {
    throw std::runtime_error(
        "invalid parent type " + parent.type.to_string() +
        " for LOG1PEXP operator at node_id " + std::to_string(node->index));
  }
}

void log(graph::Node* node) {
  assert(node->in_nodes.size() == 1);
  const graph::AtomicValue& parent = node->in_nodes[0]->value;
  if (parent.type == graph::AtomicType::POS_REAL) {
    node->value._double = std::log(parent._double);
  } else {
    throw std::runtime_error(
        "invalid parent type " +
        std::to_string(static_cast<int>(parent.type.atomic_type)) +
        " for LOG operator at node_id " + std::to_string(node->index));
  }
}

} // namespace oper
} // namespace beanmachine