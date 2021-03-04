#include <gtest/gtest.h>
#include <Tree.hh>
#include <cstdlib>

namespace TreeCellTest{

class TestClass{
public:
    TestClass(TestClass&) = delete;
    TestClass(){
        ID = id;
        id++;
    }
    ~TestClass(){
        id--;
    }
    int ID;
    static int id;
};

int TestClass::id = 0;

TEST(TreeCell_test, add_getNextOtp_getPrevOtp_getPointed_getPredestor_getSize){
    TestClass::id = 0;
    {
        TestClass** val = new TestClass*[10];
        for(unsigned int i = 0; i < 10; i++)
            val[i] = new TestClass();
        TreeCell<TestClass> treeRoot(val[0], nullptr);
        TreeCell<TestClass>* branch1 = new TreeCell<TestClass>(val[1], &treeRoot);
        branch1->add(val[8]);
        branch1->add(val[9]);
        TreeCell<TestClass>* branch2 = new TreeCell<TestClass>(val[2], &treeRoot);
        EXPECT_TRUE(treeRoot.add(branch1));
        treeRoot.add(val[3]);
        EXPECT_TRUE(treeRoot.add(branch2));
        treeRoot.add(val[4]);
        treeRoot.add(val[5]);
        treeRoot.add(val[6]);
        treeRoot.add(val[7]);
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[7]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, branch1->getVal()->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[3]->ID);
        EXPECT_EQ(treeRoot.getSize(), 7);
        EXPECT_EQ(TestClass::id, 10);
    }
    EXPECT_EQ(TestClass::id, 0);
}

TEST(TreeCell_test, incrementOtp_decrementOtp){
    TestClass::id = 0;
    {
        TestClass** val = new TestClass*[8];
        for(unsigned int i = 0; i < 8; i++)
            val[i] = new TestClass();
        TreeCell<TestClass> treeRoot(nullptr, nullptr);
        for(uint i = 0; i < 8; i++)
            treeRoot.add(val[i]);
        EXPECT_EQ(treeRoot.getSize(), 8);
        EXPECT_EQ(TestClass::id, 8);

        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[7]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[0]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[1]->ID);
        treeRoot.decrementOtp();
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[6]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[7]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[0]->ID);
        treeRoot.decrementOtp();
        treeRoot.decrementOtp();
        treeRoot.decrementOtp();
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[3]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[4]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[5]->ID);
        treeRoot.incrementOtp();
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[4]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[5]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[6]->ID);
        treeRoot.incrementOtp();
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[5]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[6]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[7]->ID);
        treeRoot.incrementOtp();
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[6]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[7]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[0]->ID);
        treeRoot.incrementOtp();
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[7]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[0]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[1]->ID);
        treeRoot.incrementOtp();
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[0]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[1]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[2]->ID);
        treeRoot.incrementOtp();
        EXPECT_EQ(treeRoot.getPrevOtp()->getVal()->ID, val[1]->ID);
        EXPECT_EQ(treeRoot.getPointed()->getVal()->ID, val[2]->ID);
        EXPECT_EQ(treeRoot.getNextOtp()->getVal()->ID, val[3]->ID);
    }
    EXPECT_EQ(TestClass::id, 0);
}

TEST(TreeCell_test, getPredestor){
    TestClass::id = 0;
    {
        TestClass** val = new TestClass*[5];
        for(unsigned int i = 0; i < 5; i++)
            val[i] = new TestClass();
        TreeCell<TestClass> treeRoot(val[0], nullptr);
        TreeCell<TestClass>* branch1 = new TreeCell<TestClass>(val[1], &treeRoot);
        TreeCell<TestClass>* branch2 = new TreeCell<TestClass>(val[2], branch1);
        TreeCell<TestClass>* branch3 = new TreeCell<TestClass>(val[3], &treeRoot);
        TreeCell<TestClass>* branch4 = new TreeCell<TestClass>(val[4], branch3);
        EXPECT_TRUE(branch1->add(branch2));
        EXPECT_TRUE(treeRoot.add(branch1));
        EXPECT_TRUE(treeRoot.add(branch3));
        EXPECT_FALSE(treeRoot.add(branch4));
        EXPECT_EQ(treeRoot.getPredestor(), nullptr);
        EXPECT_EQ(branch1->getPredestor(), &treeRoot);
        EXPECT_EQ(branch2->getPredestor(), branch1);
        EXPECT_EQ(branch3->getPredestor(), &treeRoot);
        EXPECT_EQ(treeRoot.getSize(), 2);
        EXPECT_TRUE(branch3->add(branch4));
        EXPECT_EQ(TestClass::id, 5);
    }
    EXPECT_EQ(TestClass::id, 0);
}

TEST(Tree_test, getCell){
    TestClass::id = 0;
    {
        TreeCell<TestClass>* treeRoot = new TreeCell<TestClass>(new TestClass(), nullptr);
        Tree<TestClass> tree(treeRoot);
        TreeCell<TestClass>* branch1 = new TreeCell<TestClass>(new TestClass(), treeRoot);
        TreeCell<TestClass>* branch2 = new TreeCell<TestClass>(new TestClass(), branch1);
        TreeCell<TestClass>* branch3 = new TreeCell<TestClass>(new TestClass(), treeRoot);
        TreeCell<TestClass>* branch4 = new TreeCell<TestClass>(new TestClass(), branch3);
        EXPECT_TRUE(branch1->add(branch2));
        EXPECT_TRUE(treeRoot->add(branch1));
        EXPECT_TRUE(treeRoot->add(branch3));
        EXPECT_TRUE(branch3->add(branch4));
        EXPECT_EQ(tree.getCell(), treeRoot);
        EXPECT_EQ(tree.getCell()->getPointed(), branch1);
        EXPECT_EQ(tree.getCell()->getPointed()->getPointed(), branch2);
        tree.getCell()->incrementOtp();
        EXPECT_EQ(tree.getCell(), treeRoot);
        EXPECT_EQ(tree.getCell()->getPointed(), branch3);
        EXPECT_EQ(tree.getCell()->getPointed()->getPointed(), branch4);
        EXPECT_EQ(TestClass::id, 5);
    }
    EXPECT_EQ(TestClass::id, 0);
}

TEST(Tree_test, incrementOtp_decrementOtp_incrementDir_decrementDir){
    TestClass::id = 0;
    {
        TreeCell<TestClass>* treeRoot = new TreeCell<TestClass>(nullptr, nullptr);
        Tree<TestClass> tree(treeRoot);
        TreeCell<TestClass>* branch1 = new TreeCell<TestClass>(new TestClass(), treeRoot);
        TreeCell<TestClass>* branch2 = new TreeCell<TestClass>(new TestClass(), branch1);
        TreeCell<TestClass>* branch3 = new TreeCell<TestClass>(new TestClass(), treeRoot);
        TreeCell<TestClass>* branch4 = new TreeCell<TestClass>(new TestClass(), branch3);
        EXPECT_TRUE(branch1->add(branch2));                        // root -o> 0 -o> 1 -> 6
        EXPECT_TRUE(branch3->add(branch4));                        //       |     o> 4
        EXPECT_TRUE(treeRoot->add(branch1));                       //       |     o> 5
        EXPECT_TRUE(treeRoot->add(branch3));                       //       |
        branch1->add(new TestClass());                             //       o> 2 -o> 3 -o> 8
        branch1->add(new TestClass());                             //       |     |     o> 9
        branch2->add(new TestClass());                             //       |     o> 7
        branch3->add(new TestClass());                             //       |
        branch4->add(new TestClass());                             //       o> 10
        branch4->add(new TestClass());                             //       o> 11
        treeRoot->add(new TestClass());                            //       o> 12
        treeRoot->add(new TestClass());                            //       o> 13
        treeRoot->add(new TestClass());                            //       o> 14
        treeRoot->add(new TestClass());                            // 
        treeRoot->add(new TestClass());                            // 
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 0);
        tree.incrementOtp();
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 2);
        tree.incrementOtp();
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 10);
        tree.incrementOtp();
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 11);
        tree.incrementOtp();
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 12);
        tree.incrementOtp();
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 13);
        tree.incrementOtp();
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 14);
        tree.incrementOtp();
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 0);
        tree.incrementDir();
        EXPECT_EQ(tree.getCell()->getVal(), branch1->getVal());
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 1);
        tree.decrementOtp();
        EXPECT_EQ(tree.getCell()->getVal(), branch1->getVal());
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 5);
        tree.decrementOtp();
        tree.incrementDir();
        EXPECT_EQ(tree.getCell()->getVal()->ID, 4);
        tree.decrementDir();
        ASSERT_TRUE(tree.getCell()->getSize() != 0);
        EXPECT_EQ(tree.getCell()->getVal(), branch1->getVal());
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 4);
        tree.decrementOtp();
        tree.incrementDir();
        ASSERT_TRUE(tree.getCell()->getSize() != 0);
        EXPECT_EQ(tree.getCell()->getVal(), branch2->getVal());
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 6);
        tree.incrementDir();
        tree.incrementDir();
        tree.decrementDir();
        tree.incrementOtp();
        tree.decrementDir();
        tree.decrementDir();
        ASSERT_TRUE(tree.getCell()->getSize() != 0);
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 0);
        tree.decrementDir();
        ASSERT_TRUE(tree.getCell()->getSize() != 0);
        EXPECT_EQ(tree.getCell()->getVal(), nullptr);
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 0);
        EXPECT_EQ(TestClass::id, 15);
        tree.incrementOtp();
        tree.incrementDir();
        tree.decrementOtp();
        tree.decrementOtp();
        tree.incrementDir();
        tree.incrementDir();
        tree.decrementDir();
        tree.incrementOtp();
        ASSERT_TRUE(tree.getCell()->getSize() != 0);
        EXPECT_EQ(tree.getCell()->getVal(), branch4->getVal());
        EXPECT_EQ(tree.getCell()->getPointed()->getVal()->ID, 9);
    }
    EXPECT_EQ(TestClass::id, 0);
}

}