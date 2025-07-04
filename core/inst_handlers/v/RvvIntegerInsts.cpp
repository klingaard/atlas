#include "core/inst_handlers/v/RvvIntegerInsts.hpp"
#include "core/AtlasState.hpp"
#include "core/ActionGroup.hpp"
#include "core/VecElements.hpp"
#include "include/ActionTags.hpp"

namespace atlas
{
    template <typename XLEN>
    void RvvIntegerInsts::getInstHandlers(std::map<std::string, Action> & inst_handlers)
    {
        static_assert(std::is_same_v<XLEN, RV64> || std::is_same_v<XLEN, RV32>);

        inst_handlers.emplace(
            "vadd.vv", atlas::Action::createAction<&RvvIntegerInsts::viavvHandler_<std::plus>,
                                                   RvvIntegerInsts>(nullptr, "vadd.vv",
                                                                    ActionTags::EXECUTE_TAG));
        inst_handlers.emplace(
            "vadd.vx", atlas::Action::createAction<&RvvIntegerInsts::viavxHandler_<XLEN, std::plus>,
                                                   RvvIntegerInsts>(nullptr, "vadd.vx",
                                                                    ActionTags::EXECUTE_TAG));
        inst_handlers.emplace(
            "vadd.vi", atlas::Action::createAction<&RvvIntegerInsts::viaviHandler_<std::plus>,
                                                   RvvIntegerInsts>(nullptr, "vadd.vi",
                                                                    ActionTags::EXECUTE_TAG));
    }

    template void RvvIntegerInsts::getInstHandlers<RV32>(std::map<std::string, Action> &);
    template void RvvIntegerInsts::getInstHandlers<RV64>(std::map<std::string, Action> &);

    template <size_t ElemWidth, typename Functor>
    Action::ItrType viavvHelper(AtlasState* state, Action::ItrType action_it)
    {
        const AtlasInstPtr & inst = state->getCurrentInst();
        Elements<Element<ElemWidth>, false> elems_vs1{state, state->getVectorConfig(),
                                                      inst->getRs1()};
        Elements<Element<ElemWidth>, false> elems_vs2{state, state->getVectorConfig(),
                                                      inst->getRs2()};
        Elements<Element<ElemWidth>, false> elems_vd{state, state->getVectorConfig(),
                                                     inst->getRd()};

        auto execute = [&]<typename Iterator>(const Iterator & begin, const Iterator & end)
        {
            size_t index = 0;
            for (auto iter = begin; iter != end; ++iter)
            {
                index = iter.getIndex();
                elems_vd.getElement(index).setVal(Functor()(elems_vs1.getElement(index).getVal(),
                                                            elems_vs2.getElement(index).getVal()));
            }
        };
        if (inst->getVM()) // unmasked
        {
            execute(elems_vs1.begin(), elems_vs1.end());
        }
        else // masked
        {
            const MaskElements mask_elems{state, state->getVectorConfig(), atlas::V0};
            execute(mask_elems.maskBitIterBegin(), mask_elems.maskBitIterEnd());
        }

        return ++action_it;
    }

    template <template <typename> typename FunctorTemp>
    Action::ItrType RvvIntegerInsts::viavvHandler_(AtlasState* state, Action::ItrType action_it)
    {
        VectorConfig* vector_config = state->getVectorConfig();
        switch (vector_config->getSEW())
        {
            case 8:
                return viavvHelper<8, FunctorTemp<uint8_t>>(state, action_it);
                break;
            case 16:
                return viavvHelper<16, FunctorTemp<uint16_t>>(state, action_it);
                break;
            case 32:
                return viavvHelper<32, FunctorTemp<uint32_t>>(state, action_it);
                break;
            case 64:
                return viavvHelper<64, FunctorTemp<uint64_t>>(state, action_it);
                break;
            default:
                sparta_assert(false, "Invalid SEW value");
                break;
        }
        return ++action_it;
    }

    template <typename XLEN, template <typename> typename OP>
    Action::ItrType RvvIntegerInsts::viavxHandler_(AtlasState* state, Action::ItrType action_it)
    {
        (void)state;
        return ++action_it;
    }

    template <template <typename> typename OP>
    Action::ItrType RvvIntegerInsts::viaviHandler_(AtlasState* state, Action::ItrType action_it)
    {
        (void)state;
        return ++action_it;
    }

} // namespace atlas
