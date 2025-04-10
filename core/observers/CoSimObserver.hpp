#pragma once

#include "core/observers/Observer.hpp"
#include "cosim/Event.hpp"

#include "sparta/utils/SpartaAssert.hpp"

namespace atlas
{
    class CoSimObserver : public Observer
    {
      public:
        using base_type = CoSimObserver;

        CoSimObserver();

        cosim::Event getLastEvent() const
        {
            sparta_assert(last_event_.done_ == true, "Last Event is not done yet!");
            return last_event_;
        }

        void stopSim() override
        {
            last_event_.done_ = true;
            last_event_.event_ends_sim_ = true;
        }

      private:
        ActionGroup* preExecute_(AtlasState* state) override;
        ActionGroup* preException_(AtlasState* state) override;
        ActionGroup* postExecute_(AtlasState* state) override;

        uint64_t event_uid_ = 0;
        cosim::Event last_event_ = cosim::Event(event_uid_, cosim::Event::Type::INSTRUCTION);
    };
} // namespace atlas
