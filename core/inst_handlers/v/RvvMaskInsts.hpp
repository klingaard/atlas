#pragma once

#include <map>
#include <string>
#include <stdint.h>

#include "core/Action.hpp"

namespace atlas
{
    class AtlasState;
    class Action;
    class ActionGroup;

    class RvvMaskInsts
    {
      public:
        using base_type = RvvMaskInsts;

        template <typename XLEN>
        static void getInstHandlers(std::map<std::string, Action> & inst_handlers);

        enum struct SetFirstMode
        {
            BEFORE,
            INCLUDING,
            ONLY
        };

      private:
        template <auto func>
        Action::ItrType vmlHandler_(atlas::AtlasState* state_ptr, Action::ItrType action_it);
        template <typename XLEN>
        Action::ItrType vcpHandler_(atlas::AtlasState* state_ptr, Action::ItrType action_it);
        template <typename XLEN>
        Action::ItrType vfirstHandler_(atlas::AtlasState* state_ptr, Action::ItrType action_it);
        template <SetFirstMode sfMode>
        Action::ItrType vsxfHandler_(atlas::AtlasState* state, Action::ItrType action_it);

        Action::ItrType viotaHandler_(atlas::AtlasState* state, Action::ItrType action_it);
        Action::ItrType veiHandler_(AtlasState* state, Action::ItrType action_it);
    };
} // namespace atlas
