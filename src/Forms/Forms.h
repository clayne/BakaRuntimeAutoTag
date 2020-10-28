#pragma once

namespace RE
{
	namespace InventoryInterface
	{
		struct CountChangedEvent;
		struct FavoriteChangedEvent;

		class Handle
		{
		public:
			std::uint32_t id;
		};
		static_assert(sizeof(Handle) == 0x04);
	}

	class BGSInventoryInterface :
		public BSTSingletonSDM<BGSInventoryInterface>,
		public BSTEventSource <InventoryInterface::CountChangedEvent>,
		public BSTEventSource <InventoryInterface::FavoriteChangedEvent>
	{
	public:
		struct Agent
		{
		public:
			std::uint32_t handleID;
			ObjectRefHandle itemOwner;
			std::uint16_t listIndex;
			std::uint16_t refCount;
		};
		static_assert(sizeof(Agent) == 0x0C);

		[[nodiscard]] static BGSInventoryInterface* GetSingleton()
		{
			REL::Relocation<BGSInventoryInterface**> singleton{ REL::ID(501899) };
			return *singleton;
		}

		[[nodiscard]] const BGSInventoryItem* RequestInventoryItem(const std::uint32_t& a_handleID)
		{
			using func_t = decltype(&BGSInventoryInterface::RequestInventoryItem);
			REL::Relocation<func_t> func{ REL::ID(1200959) };
			return func(this, a_handleID);
		}

		// members
		BSTArray<Agent> agentArray;	 // 00
	};
	static_assert(sizeof(BGSInventoryInterface) == 0xD0);

	class InventoryUserUIInterfaceEntry
	{
	public:
		InventoryInterface::Handle invHandle;
		BSTSmallArray<std::uint16_t, 4> stackIndex;
	};
	static_assert(sizeof(InventoryUserUIInterfaceEntry) == 0x20);
}

class Forms
{
private:
	static bool BSString__Set(RE::BSStringT<char>* a_this, const char* a_string, std::uint16_t a_maxSize = 0)
	{
		using func_t = bool (RE::BSStringT<char>::*)(const char*, std::uint16_t);
		REL::Relocation<func_t> func{ REL::ID(407804) };
		return func(a_this, a_string, a_maxSize);
	}

	static std::uint32_t HookInitializer_DefaultObject()
	{
		// Initializer override
		AmmoWornKeyword_DO =
			RE::DefaultObjectFormFactory::Create(
				"AmmoWornKeyword_DO",
				"This ammo is worn on the actors body.",
				RE::ENUM_FORM_ID::kKYWD);

		// Add new
		BakaSortDO_KYWD_Armor =
			RE::DefaultObjectFormFactory::Create(
				"BakaSortDO_KYWD_Armor",
				RE::ENUM_FORM_ID::kKYWD);

		BakaSortDO_KYWD_ArmorDog =
			RE::DefaultObjectFormFactory::Create(
				"BakaSortDO_KYWD_ArmorDog",
				RE::ENUM_FORM_ID::kKYWD);

		BakaSortDO_KYWD_ArmorPA =
			RE::DefaultObjectFormFactory::Create(
				"BakaSortDO_KYWD_ArmorPA",
				RE::ENUM_FORM_ID::kKYWD);

		BakaSortDO_KYWD_Bobblehead =
			RE::DefaultObjectFormFactory::Create(
				"BakaSortDO_KYWD_Bobblehead",
				RE::ENUM_FORM_ID::kKYWD);

		BakaSortDO_KYWD_Firework =
			RE::DefaultObjectFormFactory::Create(
				"BakaSortDO_KYWD_Firework",
				RE::ENUM_FORM_ID::kKYWD);

		BakaSortDO_KYWD_FusionCore =
			RE::DefaultObjectFormFactory::Create(
				"BakaSortDO_KYWD_FusionCore",
				RE::ENUM_FORM_ID::kKYWD);

		BakaSortDO_KYWD_LooseMod =
			RE::DefaultObjectFormFactory::Create(
				"BakaSortDO_KYWD_LooseMod",
				RE::ENUM_FORM_ID::kKYWD);

		BakaSortDO_FLST_Bottle =
			RE::DefaultObjectFormFactory::Create(
				"BakaSortDO_FLST_Bottle",
				RE::ENUM_FORM_ID::kFLST);

		logger::debug("Injected DefaultObjects."sv);
		return 1;
	}

	static std::uint32_t HookInitializer_Setting()
	{
		RE::GameSettingCollection::InitCollection();
		if (auto GameSettingCollection = RE::GameSettingCollection::GetSingleton(); GameSettingCollection)
		{
			// Initializer override
			GameSettingCollection->Add(&fMagicAreaScale);

			// Add new
			GameSettingCollection->Add(&sBakaSortPrefixAmmo);
			GameSettingCollection->Add(&sBakaSortPrefixBobblehead);
			GameSettingCollection->Add(&sBakaSortPrefixBottle);
			GameSettingCollection->Add(&sBakaSortPrefixComponent);
			GameSettingCollection->Add(&sBakaSortPrefixCraftingItem);
			GameSettingCollection->Add(&sBakaSortPrefixCurrency);
			GameSettingCollection->Add(&sBakaSortPrefixFusionCore);
			GameSettingCollection->Add(&sBakaSortPrefixHolotape);
			GameSettingCollection->Add(&sBakaSortPrefixIDCard);
			GameSettingCollection->Add(&sBakaSortPrefixKey);
			GameSettingCollection->Add(&sBakaSortPrefixLockpick);
			GameSettingCollection->Add(&sBakaSortPrefixMagazine);
			GameSettingCollection->Add(&sBakaSortPrefixMod);
			GameSettingCollection->Add(&sBakaSortPrefixNote);
			GameSettingCollection->Add(&sBakaSortPrefixScrap);
			GameSettingCollection->Add(&sBakaSortPrefixTrash);
		}

		logger::debug("Injected GMSTs."sv);
		return 1;
	}

	static void PopulateMenuObj(RE::ObjectRefHandle* a_invRef, const RE::InventoryUserUIInterfaceEntry& a_entry, RE::Scaleform::GFx::Value& a_menuObj)
	{
		_PopulateMenuObj(a_invRef, a_entry, a_menuObj);
		
		if (a_menuObj.IsObject())
		{
			if (!a_menuObj.HasMember("text"))
			{
				return;
			}

			RE::Scaleform::GFx::Value menuObjText;
			a_menuObj.GetMember("text", &menuObjText);
			if (!menuObjText.IsString())
			{
				return;
			}

			auto item = RE::BGSInventoryInterface::GetSingleton()->RequestInventoryItem(a_entry.invHandle.id);
			if (item && item->object && item->stackData && item->stackData->extra)
			{
				auto extra = item->stackData->extra->GetByType<RE::ExtraInstanceData>();
				auto instanceData = extra ? extra->data.get() : nullptr;

				switch (item->object->formType.get())
				{
				case RE::ENUM_FORM_ID::kARMO:
					{
						auto armo = item->object->As<RE::TESObjectARMO>();
						auto prefix = [&]() -> std::string_view {
							if (auto kywd = BakaSortDO_KYWD_ArmorDog->GetForm<RE::BGSKeyword>(); kywd)
							{
								if (armo->HasKeyword(kywd, instanceData))
								{
									return sBakaSortPrefixArmorDog.GetString();
								}
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kBody))
							{
								if (armo->bipedModelData.bipedObjectSlots.anyBits(
										RE::BIPED_OBJECT::kArmL,
										RE::BIPED_OBJECT::kArmR,
										RE::BIPED_OBJECT::kLegL,
										RE::BIPED_OBJECT::kLegR))
								{
									return sBakaSortPrefixArmorClothing.GetString();
								}

								return sBakaSortPrefixArmorUnderarmor.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kBeard,
									RE::BIPED_OBJECT::kMouth))
							{
								return sBakaSortPrefixArmorMask.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kHairTop,
									RE::BIPED_OBJECT::kHairLong,
									RE::BIPED_OBJECT::kHeadband))
							{
								if (auto kywd = BakaSortDO_KYWD_ArmorPA->GetForm<RE::BGSKeyword>(); kywd)
								{
									if (armo->HasKeyword(kywd, instanceData))
									{
										return sBakaSortPrefixArmorPowerArmorHelmet.GetString();
									}
								}

								if (auto kywd = BakaSortDO_KYWD_Armor->GetForm<RE::BGSKeyword>(); kywd)
								{
									if (armo->HasKeyword(kywd, instanceData))
									{
										return sBakaSortPrefixArmorHelmet.GetString();
									}
								}

								return sBakaSortPrefixArmorHat.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kTorsoA))
							{
								if (auto kywd = BakaSortDO_KYWD_ArmorPA->GetForm<RE::BGSKeyword>(); kywd)
								{
									if (armo->HasKeyword(kywd, instanceData))
									{
										return sBakaSortPrefixArmorPowerArmorChest.GetString();
									}
								}

								return sBakaSortPrefixArmorChest.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kArmLA))
							{
								if (auto kywd = BakaSortDO_KYWD_ArmorPA->GetForm<RE::BGSKeyword>(); kywd)
								{
									if (armo->HasKeyword(kywd, instanceData))
									{
										return sBakaSortPrefixArmorPowerArmorArmL.GetString();
									}
								}

								return sBakaSortPrefixArmorArmL.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kArmRA))
							{
								if (auto kywd = BakaSortDO_KYWD_ArmorPA->GetForm<RE::BGSKeyword>(); kywd)
								{
									if (armo->HasKeyword(kywd, instanceData))
									{
										return sBakaSortPrefixArmorPowerArmorArmR.GetString();
									}
								}

								return sBakaSortPrefixArmorArmR.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kLegLA))
							{
								if (auto kywd = BakaSortDO_KYWD_ArmorPA->GetForm<RE::BGSKeyword>(); kywd)
								{
									if (armo->HasKeyword(kywd, instanceData))
									{
										return sBakaSortPrefixArmorPowerArmorLegL.GetString();
									}
								}

								return sBakaSortPrefixArmorLegL.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kLegRA))
							{
								if (auto kywd = BakaSortDO_KYWD_ArmorPA->GetForm<RE::BGSKeyword>(); kywd)
								{
									if (armo->HasKeyword(kywd, instanceData))
									{
										return sBakaSortPrefixArmorPowerArmorLegR.GetString();
									}
								}

								return sBakaSortPrefixArmorLegR.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kEyes))
							{
								return sBakaSortPrefixArmorEyes.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kRing))
							{
								return sBakaSortPrefixArmorRing.GetString();
							}

							if (armo->bipedModelData.bipedObjectSlots.anyBits(
									RE::BIPED_OBJECT::kPipboy))
							{
								return sBakaSortPrefixArmorPipboy.GetString();
							}

							return ""sv;
						}();

						if (prefix.size() == 0)
						{
							logger::info("Skipping (No Prefix): {:08X}", item->object->formID);
							break;
						}

						auto text = fmt::format("{} {}", prefix, menuObjText.GetString());
						a_menuObj.SetMember("text", text.c_str());
						break;
					}

				case RE::ENUM_FORM_ID::kWEAP:
					{
						break;
					}

				default:
					break;
				}

			}
			else
			{
				if (item)
				{
					if (!item->object)
					{
						logger::info("Skipping (No Object): {:08X}", item->object->formID);
						return;
					}

					if (!item->stackData)
					{
						logger::info("Skipping (No stackData): {:08X}", item->object->formID);
						return;
					}

					if (!item->stackData->extra)
					{
						logger::info("Skipping (No extra): {:08X}", item->object->formID);
						return;
					}

					logger::info("This should never happen");
				}
				else
				{
					logger::info("Skipping (No Item): {:08X}", item->object->formID);
				}
			}
		}
	}

	// members
	inline static RE::Setting fMagicAreaScale{ "fMagicAreaScale", 1.75f };
	inline static RE::BGSDefaultObject* AmmoWornKeyword_DO{ nullptr };

	inline static REL::Relocation<decltype(PopulateMenuObj)> _PopulateMenuObj;

public:
	static void InstallHooks()
	{
		REL::Relocation<std::uintptr_t> target_DefaultObject{ REL::ID(1359842) };
		REL::Relocation<std::uintptr_t> target_Setting{ REL::ID(1411720) };
		REL::Relocation<std::uintptr_t> target_PopulateMenuObj{ REL::ID(969445), 0x53 };

		auto& trampoline = F4SE::GetTrampoline();
		trampoline.write_branch<6>(target_DefaultObject.address(), HookInitializer_DefaultObject);
		trampoline.write_branch<5>(target_Setting.address(), HookInitializer_Setting);
		
		_PopulateMenuObj = trampoline.write_call<5>(target_PopulateMenuObj.address(), PopulateMenuObj);
	}

	static void ApplyTags()
	{
		// Start
		logger::debug("Starting static Auto-Tag."sv);

		// Lists
		std::vector<std::uint32_t> miscUsedByCMPO;
		std::vector<std::uint32_t> miscUsedByCOBJ;

		// Get DataHandler
		const auto data = RE::TESDataHandler::GetSingleton();
		if (!data)
		{
			logger::error("Failed to get TESDataHandler."sv);
			return;
		}

		// ALCH
		logger::debug("Processing ALCH forms."sv);
		// for (auto form : data->GetFormArray<RE::AlchemyItem>())
		// {
		// 
		// }

		// AMMO
		logger::debug("Processing AMMO forms."sv);
		for (auto form : data->GetFormArray<RE::TESAmmo>())
		{
			auto name = fmt::format(
				"{} {}"s,
				[&]() -> std::string_view
				{
					if (auto kywd = BakaSortDO_KYWD_FusionCore->GetForm<RE::BGSKeyword>(); kywd)
					{
						if (form->HasKeyword(kywd, nullptr))
						{
							return sBakaSortPrefixFusionCore.GetString();
						}
					}

					return sBakaSortPrefixAmmo.GetString();
				}(),
				form->GetFullName());

			form->SetFullName(name.c_str());
		}

		// BOOK
		logger::debug("Processing BOOK forms."sv);
		for (auto form : data->GetFormArray<RE::TESObjectBOOK>())
		{
			auto name = fmt::format(
				"{} {}"s,
				[&]() -> std::string_view {
					auto trns = form->previewTransform.transform;
					if (trns)
					{
						switch (trns->formID)
						{
						case 0x001BB354:  // MiscMagazine
						case 0x001E2EAD:  // Misc_MagazineFix
							return sBakaSortPrefixMagazine.GetString();
						}
					}

					return sBakaSortPrefixNote.GetString();
				}(),
				form->GetFullName());

			form->SetFullName(name.c_str());
		}

		// COBJ
		logger::debug("Processing COBJ forms to build MiscObject list."sv);
		for (auto form : data->GetFormArray<RE::BGSConstructibleObject>())
		{
			if (form->requiredItems)
			{
				for (auto item : *form->requiredItems)
				{
					auto misc = item.first->As<RE::TESObjectMISC>();
					if (misc)
					{
						miscUsedByCOBJ.emplace_back(misc->formID);
					}
				}
			}
		}

		// CMPO
		logger::debug("Processing CMPO forms."sv);
		for (auto form : data->GetFormArray<RE::BGSComponent>())
		{
			auto name = fmt::format(
				"{} {}"s,
				sBakaSortPrefixComponent.GetString(),
				form->GetFullName());

			if (form->scrapItem)
			{
				miscUsedByCMPO.emplace_back(form->scrapItem->formID);
			}

			form->SetFullName(name.c_str());
		}

		// KEYM
		logger::debug("Processing KEYM forms."sv);
		for (auto form : data->GetFormArray<RE::TESKey>())
		{
			auto name = fmt::format(
				"{} {}"s,
				[&]() -> std::string_view {
					auto trns = form->previewTransform.transform;
					if (trns)
					{
						switch (trns->formID)
						{
						case 0x000995A5:  // MiscHolotape
							return sBakaSortPrefixHolotape.GetString();
						case 0x00248895:  // MiscIDCard
							return sBakaSortPrefixIDCard.GetString();
						case 0x00248896:  // MiscNoteKey
							return sBakaSortPrefixNote.GetString();
						}
					}

					return sBakaSortPrefixKey.GetString();
				}(),
				form->GetFullName());

			form->SetFullName(name.c_str());
		}

		// MISC
		logger::debug("Processing MISC forms."sv);
		for (auto form : data->GetFormArray<RE::TESObjectMISC>())
		{
			std::stringstream componentList;
			if (form->componentData)
			{
				auto numComponents = form->componentData->size();
				if (numComponents > 0)
				{
					componentList << "{{{";
					for (std::uint32_t i = 0; i < numComponents;)
					{
						auto cmpo = (*form->componentData)[i];
						auto name = RE::TESFullName::GetFullName(*cmpo.first);

						auto pre = sBakaSortPrefixComponent.GetString();
						auto idx = name.find(pre);
						if (idx != std::string_view::npos)
						{
							name = name.substr(pre.size() + 1, name.size());
						}

						componentList << name << ((++i != numComponents) ? ", " : "}}}");
					}
				}
			}

			auto componentString = componentList.str();
			auto name = fmt::format(
				"{} {}{}"s,
				[&]() -> std::string_view {
					auto trns = form->previewTransform.transform;
					if (trns)
					{
						switch (trns->formID)
						{
						case 0x000995A5:  // MiscHolotape
							return sBakaSortPrefixHolotape.GetString();
						case 0x00248895:  // MiscIDCard
							return sBakaSortPrefixIDCard.GetString();
						case 0x001CF026:  // MiscFolder
						case 0x0021505E:  // MiscNewspaper
						case 0x0023E9E1:  // MiscRecipe
						case 0x002488FF:  // MiscNote02
						case 0x0100363E:  // DLC01Mq02Schematics_Transform
							return sBakaSortPrefixNote.GetString();
						}
					}

					switch (form->formID)
					{
					case 0x0000000F:  // Caps001
					case 0x0010DEE5:  // DN011OverdueBookReturnToken
					case 0x00059AEA:  // SubwayTicket
					case 0x00059B02:  // PrewarMoney
					case 0x06019988:  // DLC04_NukaCade_Ticket01
					case 0x0601998A:  // DLC04_NukaCade_Token01
						return sBakaSortPrefixCurrency.GetString();
					case 0x0000000A:  // BobbyPin
					case 0x0008FCA8:  // BobbyPinBox
						return sBakaSortPrefixLockpick.GetString();
					}

					if (auto kywd = BakaSortDO_KYWD_LooseMod->GetForm<RE::BGSKeyword>(); kywd)
					{
						if (form->HasKeyword(kywd, nullptr))
						{
							return sBakaSortPrefixMod.GetString();
						}
					}

					if (auto kywd = BakaSortDO_KYWD_Bobblehead->GetForm<RE::BGSKeyword>(); kywd)
					{
						if (form->HasKeyword(kywd, nullptr))
						{
							return sBakaSortPrefixBobblehead.GetString();
						}
					}

					if (auto kywd = BakaSortDO_KYWD_Firework->GetForm<RE::BGSKeyword>(); kywd)
					{
						if (form->HasKeyword(kywd, nullptr))
						{
							return sBakaSortPrefixAmmo.GetString();
						}
					}

					if (auto flst = BakaSortDO_FLST_Bottle->GetForm<RE::BGSListForm>(); flst)
					{
						if (flst->ContainsItem(form))
						{
							return sBakaSortPrefixMod.GetString();
						}
					}

					if (std::find(miscUsedByCMPO.begin(), miscUsedByCMPO.end(), form->formID) != miscUsedByCMPO.end())
					{
						return sBakaSortPrefixComponent.GetString();
					}

					if (std::find(miscUsedByCOBJ.begin(), miscUsedByCOBJ.end(), form->formID) != miscUsedByCOBJ.end())
					{
						return sBakaSortPrefixCraftingItem.GetString();
					}

					return componentString.empty() ? sBakaSortPrefixTrash.GetString() : sBakaSortPrefixScrap.GetString();
				}(),
				form->GetFullName(),
				componentString);

			form->SetFullName(name.c_str());
		}

		// NOTE
		logger::debug("Processing NOTE forms."sv);
		for (auto form : data->GetFormArray<RE::BGSNote>())
		{
			auto name = fmt::format(
				"{} {}"s,
				sBakaSortPrefixHolotape.GetString(),
				form->GetFullName());

			form->SetFullName(name.c_str());
		}

		logger::debug("Finished static Auto-Tag."sv);
	}

private:
	inline static RE::Setting sBakaSortPrefixAmmo{
		"sBakaSortPrefixAmmo",
		"[W1.Ammo]"
	};

	inline static RE::Setting sBakaSortPrefixArmorArmL{
		"sBakaSortPrefixArmorArmL",
		"[A4.ArmL]"
	};

	inline static RE::Setting sBakaSortPrefixArmorArmR{
		"sBakaSortPrefixArmorArmR",
		"[A4.ArmR]"
	};

	inline static RE::Setting sBakaSortPrefixArmorChest{
		"sBakaSortPrefixArmorChest",
		"[A4.Chest]"
	};

	inline static RE::Setting sBakaSortPrefixArmorClothing{
		"sBakaSortPrefixArmorClothing",
		"[A2.Clothing]"
	};

	inline static RE::Setting sBakaSortPrefixArmorDog{
		"sBakaSortPrefixArmorDog",
		"[A6.Dog]"
	};

	inline static RE::Setting sBakaSortPrefixArmorEyes{
		"sBakaSortPrefixArmorEyes",
		"[A1.Eyes]"
	};

	inline static RE::Setting sBakaSortPrefixArmorHat{
		"sBakaSortPrefixArmorHat",
		"[A1.Hat]"
	};

	inline static RE::Setting sBakaSortPrefixArmorHelmet{
		"sBakaSortPrefixArmorHat",
		"[A4.Helmet]"
	};

	inline static RE::Setting sBakaSortPrefixArmorLegL{
		"sBakaSortPrefixArmorLegL",
		"[A4.LegL]"
	};

	inline static RE::Setting sBakaSortPrefixArmorLegR{
		"sBakaSortPrefixArmorLegR",
		"[A4.LegR]"
	};

	inline static RE::Setting sBakaSortPrefixArmorMask{
		"sBakaSortPrefixArmorMask",
		"[A3.Mask]"
	};

	inline static RE::Setting sBakaSortPrefixArmorPipboy{
		"sBakaSortPrefixArmorPipBoy",
		"[A0.Pipboy]"
	};

	inline static RE::Setting sBakaSortPrefixArmorPowerArmorArmL{
		"sBakaSortPrefixArmorPowerArmorArmL",
		"[A5.PowerArmorArmL]"
	};

	inline static RE::Setting sBakaSortPrefixArmorPowerArmorArmR{
		"sBakaSortPrefixArmorPowerArmorArmR",
		"[A5.PowerArmorArmR]"
	};

	inline static RE::Setting sBakaSortPrefixArmorPowerArmorChest{
		"sBakaSortPrefixArmorPowerArmorChest",
		"[A5.PowerArmorChest]"
	};

	inline static RE::Setting sBakaSortPrefixArmorPowerArmorHelmet{
		"sBakaSortPrefixArmorPowerArmorHelmet",
		"[A5.PowerArmorHelmet]"
	};

	inline static RE::Setting sBakaSortPrefixArmorPowerArmorLegL{
		"sBakaSortPrefixArmorPowerArmorLegL",
		"[A5.PowerArmorLegL]"
	};

	inline static RE::Setting sBakaSortPrefixArmorPowerArmorLegR{
		"sBakaSortPrefixArmorPowerArmorLegR",
		"[A5.PowerArmorLegR]"
	};

	inline static RE::Setting sBakaSortPrefixArmorRing{
		"sBakaSortPrefixArmorRing",
		"[A1.Ring]"
	};

	inline static RE::Setting sBakaSortPrefixArmorUnderarmor{
		"sBakaSortPrefixArmorUnderarmor",
		"[A2.Underarmor]"
	};

	inline static RE::Setting sBakaSortPrefixBobblehead{
		"sBakaSortPrefixBobblehead",
		"{M7.Bobblehead}"
	};

	inline static RE::Setting sBakaSortPrefixBottle{
		"sBakaSortPrefixBottle",
		"{M4.Bottle}"
	};

	inline static RE::Setting sBakaSortPrefixComponent{
		"sBakaSortPrefixComponent",
		"{M4.Component}"
	};

	inline static RE::Setting sBakaSortPrefixCraftingItem{
		"sBakaSortPrefixCraftingItem",
		"{M3.CraftingItem}"
	};

	inline static RE::Setting sBakaSortPrefixCurrency{
		"sBakaSortPrefixCurrency",
		"{M2.Currency}"
	};

	inline static RE::Setting sBakaSortPrefixFusionCore{
		"sBakaSortPrefixFusionCore",
		"[W1.FusionCore]"
	};

	inline static RE::Setting sBakaSortPrefixHolotape{
		"sBakaSortPrefixHolotape",
		"{M6.Holotape}"
	};

	inline static RE::Setting sBakaSortPrefixIDCard{
		"sBakaSortPrefixIDCard",
		"{M8.IDCard}"
	};

	inline static RE::Setting sBakaSortPrefixKey{
		"sBakaSortPrefixKey",
		"{M7.Key}"
	};

	inline static RE::Setting sBakaSortPrefixLockpick{
		"sBakaSortPrefixLockpick",
		"{M2.Lockpick}"
	};

	inline static RE::Setting sBakaSortPrefixMagazine{
		"sBakaSortPrefixMagazine",
		"{M7.Magazine}"
	};

	inline static RE::Setting sBakaSortPrefixMod{
		"sBakaSortPrefixMod",
		"{M9.Mod}"
	};

	inline static RE::Setting sBakaSortPrefixNote{
		"sBakaSortPrefixNote",
		"{M6.Note}"
	};

	inline static RE::Setting sBakaSortPrefixScrap{
		"sBakaSortPrefixScrap",
		"{M4.Scrap}"
	};

	inline static RE::Setting sBakaSortPrefixTrash{
		"sBakaSortPrefixTrash",
		"{M9.Trash}"
	};

	inline static RE::BGSDefaultObject* BakaSortDO_KYWD_Armor{ nullptr };
	inline static RE::BGSDefaultObject* BakaSortDO_KYWD_ArmorDog{ nullptr };
	inline static RE::BGSDefaultObject* BakaSortDO_KYWD_ArmorPA{ nullptr };

	inline static RE::BGSDefaultObject* BakaSortDO_KYWD_Bobblehead{ nullptr };
	inline static RE::BGSDefaultObject* BakaSortDO_KYWD_Firework{ nullptr };
	inline static RE::BGSDefaultObject* BakaSortDO_KYWD_FusionCore{ nullptr };
	inline static RE::BGSDefaultObject* BakaSortDO_KYWD_LooseMod{ nullptr };
	inline static RE::BGSDefaultObject* BakaSortDO_FLST_Bottle{ nullptr };
};