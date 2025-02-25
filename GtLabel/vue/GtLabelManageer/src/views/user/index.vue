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
        <el-table-column label="用户角色" align="center">
          质检员
        </el-table-column>
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
            <el-form-item
              :label="!isEdit ? '用户密码' : '新密码'"
              prop="password"
            >
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
import { ref, nextTick, onMounted } from "vue";
import { ElNotification } from "element-plus";

import userApi from "@/api/user";

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

let userArr = ref<any[]>([]);

const addUser = async () => {
  isEdit.value = false;
  //清空数据
  Object.assign(ruleForm.value, {
    username: "",
    nickname: "",
    password: "",
  });
  //清除上一次的错误的提示信息
  nextTick(() => {
    if (ruleFormRef.value) ruleFormRef.value.clearValidate();
  });
  drawer.value = !drawer.value;
};

const cancelClick = () => {
  drawer.value = false;
};

const confirmClick = async () => {
  drawer.value = false;
  if (!isEdit.value) {
    let res: any = await userApi.reqAddUser(ruleForm.value);
    if (res.code !== 200) {
      ElNotification({
        type: "error",
        message: "添加用户失败",
      });
    }
  }else {
    await userApi.reqUpdateUser(ruleForm.value);
  }
  await initData();
};

const deleteUser = async (id:number) => {
  let res = await userApi.reqDeleteUser({id:id});
  if (res.code !== 200) {
      ElNotification({
        type: "error",
        message: "删除用户失败",
      });
    }
    await initData();
}

const getHasUser = (pager = 1) => {
  pageNo.value = pager;
  initData();
};

const pageSizeChange = () => {
  getHasUser();
};

const updateUser = (raw: any) => {
  if(ruleFormRef.value)
    ruleFormRef.value.clearValidate();
  isEdit.value = true;
  Object.assign(ruleForm.value, {
    username: raw.username,
    nickname: raw.nickname,
    password: raw.password,
    id: raw.id
  });
  drawer.value = !drawer.value;
};

const initData = async () => {
  let res: any = await userApi.reqUsercount();
  let data: any = await UserInfo();
  if ((data.code = 200)) {
    userArr.value = data.data;
  }
  total.value = res.count;
};

const UserInfo = async () => {
  return await userApi.reqUserInfo(
    (pageNo.value - 1) * pageSize.value,
    pageSize.value
  );
};

onMounted(async () => {
  await initData();
});
</script>

<style lang="scss">
</style>