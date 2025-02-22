<template>
  <div>
    <el-button type="primary" size="default" @click="addUser"
      >添加用户</el-button
    >
    <el-card style="margin-top: 20px">
      <el-table border :data="userArr" style="margin-bottom: 20px">
        <el-table-column
          label="#"
          align="center"
          type="index"
        ></el-table-column>
        <el-table-column
          label="用户名称"
          align="center"
          prop="username"
        ></el-table-column>
        <el-table-column
          label="用户名字"
          align="center"
          prop="nickname"
        ></el-table-column>
        <el-table-column
          label="用户角色"
          align="center"

        > 质检员123 </el-table-column>
        <el-table-column
          label="创建时间"
          align="center"
          prop="createtime"
        ></el-table-column>
        <el-table-column label="操作" align="center">
          <template #="{ row, index }">
            <el-button
              type="primary"
              size="small"
              icon="Edit"
              @click="updateUser(row)"
              >编辑</el-button
            >
            <el-popconfirm
              :title="`你确定要删除${row.username}?`"
              width="260px"
              @confirm="deleteUser(row.id)"
            >
              <template #reference>
                <el-button type="primary" size="small" icon="Delete"
                  >删除</el-button
                >
              </template>
            </el-popconfirm>
          </template>
        </el-table-column>
      </el-table>
      <!-- 分页器 -->
      <el-pagination
        v-model:current-page="pageNo"
        v-model:page-size="pageSize"
        :page-sizes="[5, 7, 9, 11]"
        :background="true"
        layout="prev, pager, next, jumper,->,sizes,total"
        :total="total"
        @current-change="getHasUser"
        @size-change="pageSizeChange"
      />
    </el-card>
    <el-drawer v-model="drawer" direction="rtl">
      <template #header>
        <h4>添加用户</h4>
      </template>
      <template #default>
        <div>
          <el-form
            ref="ruleFormRef"
            label-width="auto"
            :model="ruleForm"
            :rules="rules"
          >
            <el-form-item label="用户名称" prop="username">
              <el-input v-model="ruleForm.username" />
            </el-form-item>
            <el-form-item label="用户名字" prop="nickname">
              <el-input v-model="ruleForm.nickname" />
            </el-form-item>
            <el-form-item :label="!isEdit ? '用户密码':'新密码'" prop="password">
              <el-input v-model="ruleForm.password" type="password" />
            </el-form-item>
          </el-form>
        </div>
      </template>
      <template #footer>
        <div style="flex: auto">
          <el-button @click="cancelClick">取消</el-button>
          <el-button type="primary" @click="confirmClick">确认</el-button>
        </div>
      </template>
    </el-drawer>
  </div>
</template>

<script setup lang="ts">
import { ref, nextTick ,onMounted } from "vue";

import  userApi  from '@/api/user'

let isEdit = ref(false);

let ruleFormRef = ref<any>();

let ruleForm = ref<any>({
  username: "",
  nickname: "",
  password: "",
});

const rules = ref<any>({
  username: [
    { required: true, message: "请输入用户名称", trigger: "blur" },
    { min: 2, max: 10, message: "长度范围2~10", trigger: "blur" },
  ],
  nickname: [
    { required: true, message: "请输入用户名字", trigger: "blur" },
    { min: 2, max: 10, message: "长度范围2~10", trigger: "blur" },
  ],
  password: [
    { required: true, message: "请输入用户密码", trigger: "blur" },
    { min: 2, max: 10, message: "长度范围2~10", trigger: "blur" },
  ],
});

let drawer = ref(false);
//默认页码
let pageNo = ref<number>(1);
//一页展示几条数据
let pageSize = ref<number>(5);
//用户总个数
let total = ref<number>(0);

let userArr = ref<any[]>([
  {
    id: 1,
    username: "fdd",
    nickname: "范冬冬",
    role: "质检员",
    createtime: "2025-01-21 10:45:59",
    password:"123456",
  },
  {
    id: 2,
    username: "chpan",
    nickname: "潘承浩",
    role: "质检员",
    createtime: "2025-01-21 10:45:59",
    password:"123456",
  },
  {
    id: 3,
    username: "dyw",
    nickname: "丁益文",
    role: "质检员",
    createtime: "2025-01-21 10:45:59",
    password:"123456",
  },
]);

const addUser = () => {
  isEdit.value=false;
  //清空数据
  Object.assign(ruleForm.value, {
    username: "",
    nickname: "",
    password: "",
  });
  //清除上一次的错误的提示信息
  nextTick(() => {
    ruleFormRef.value.clearValidate();
  });
  drawer.value = !drawer.value;
};

const cancelClick = () => {
  drawer.value = false;
};

const confirmClick = () => {
  drawer.value = false;
};

const getHasUser = () => {
};

const pageSizeChange = () => {};

const updateUser = (raw: any) => {
  ruleFormRef.value.clearValidate();
  isEdit.value=true;
  Object.assign(ruleForm.value, {
    username: raw.username,
    nickname: raw.nickname,
    password: raw.password,
  });
  drawer.value = !drawer.value;
};

onMounted(async() =>{
    let res:any = await userApi.reqUsercount();
    total.value=res.count;
});

</script>

<style lang="scss">
</style>