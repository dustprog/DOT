set(INCLUDES
../DOT_Alpha/Interface/AdvertisementBase.h
../DOT_Alpha/Interface/EntityBase.h
../DOT_Alpha/Interface/TagBase.h
../DOT_Alpha/Interface/InterfaceBase.h
../DOT_Alpha/Interface/TObject.h
../DOT_Alpha/Managers/BranchPredictor.h
../DOT_Alpha/DOTbuilder/InterfaceBuilder.h
../DOT_Alpha/MathUtility/Sigmoid.h
../ZETA_Alpha/Containers/TemplateTreeAddress.h
../ZETA_Alpha/Containers/SearchTree.h
../ZETA_Alpha/Containers/Node.h
../ZETA_Alpha/Containers/Containers.h
../ZETA_Alpha/Containers/GenericContainer.h
../ZETA_Alpha/Containers/NibbleContainer.h
../ZETA_Alpha/Manager/SearchTreeManager.h
../ZETA_Alpha/Manager/ThreadManager.h
../ZETA_Alpha/Processing/ProcessBase.h
)
add_executable(test_zero TestingGrounds.cpp ${INCLUDES})


target_link_libraries(test_zero DOT_Alpha ${LINK_WITH})

