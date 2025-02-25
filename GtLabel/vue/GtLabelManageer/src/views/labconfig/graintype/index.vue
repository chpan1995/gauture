<template>
  <div class="main_contain">
    <el-button
      type="primary"
      style="width: 100px"
      @click="addGrainType"
      :disabled="isEdit"
    >
      新增
    </el-button>
    <el-card class="contain">
      <template v-for="(item, index) in graintypeList" :key="index">
        <div class="lab">
          <span style="margin-right: 10px; width: 40px">种类</span>
          <el-input
            class="elinput"
            :input-style="{ textAlign: 'center' }"
            v-model="graintypeList[index].name"
            :disabled="!isEdit"
          ></el-input>

          <el-popconfirm
            :title="`你确定要删除${graintypeList[index].name}?`"
            width="260px"
            @confirm="deleteType(graintypeList[index].id)"
          >
            <template #reference>
              <el-button
                style="
                  width: 24px;
                  height: 24px;
                  border: none;
                  margin-left: 10px;
                "
                circle
              >
                <el-icon :size="24" color="#FF0000"><Remove /></el-icon>
              </el-button>
            </template>
          </el-popconfirm>
        </div>
      </template>
      <div class="tmp" v-if="isAdd">
        <span style="margin-right: 10px; width: 40px">种类</span>
        <el-input
          class="elinput"
          :input-style="{ textAlign: 'center' }"
          v-model="newGraintype"
          style="width: 360px; height: 40px"
        ></el-input>
      </div>
      <div>
        <el-button type="primary" style="width: 100px" @click="saveGrainType">
          保存
        </el-button>
        <!-- <el-button
          type="primary"
          style="width: 100px"
          @click="editGrainType"
          :disabled="isAdd"
        >
          编辑
        </el-button> -->
      </div>
    </el-card>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from "vue";
import { ElMessageBox } from "element-plus";
import grainTypeApi from "@/api/graintype";
import { animateScrollTo } from "element-plus/es/utils";

const graintypeList = ref<any[]>([]);

const isAdd = ref<boolean>(false);
const isEdit = ref<boolean>(false);

const newGraintype = ref<string>("");

const addGrainType = () => {
  isAdd.value = true;
};

const saveGrainType = async () => {
  if (isAdd.value) {
    if (newGraintype.value == "") {
      ElMessageBox.alert("输入不能为空", { confirmButtonText: "确定" });
    } else {
      isAdd.value = false;
      let res: any = await grainTypeApi.reqAddType({
        name: newGraintype.value,
      });
      if (res.code !== 200) {
        ElMessageBox.alert("添加种类失败", { confirmButtonText: "确定" });
      }
      newGraintype.value = "";
      let data: any = await grainTypeApi.reqTypeInfo();
      graintypeList.value = data.data;
    }
    return;
  }

  if (isEdit.value) {
    graintypeList.value.forEach((element) => {
      if (element == "") {
        ElMessageBox.alert("输入不能为空", { confirmButtonText: "确定" });
      }
      return;
    });
    isEdit.value = false;
  }
};

const deleteType = async (id: number) => {
  let res: any = await grainTypeApi.reqDeleteType({ id: id });
  if (res.code !== 200) {
    ElMessageBox.alert("删除种类失败", { confirmButtonText: "确定" });
  }
  let data: any = await grainTypeApi.reqTypeInfo();
  graintypeList.value = data.data;
};

const editGrainType = () => {
  isEdit.value = true;
};

onMounted(async () => {
  let data: any = await grainTypeApi.reqTypeInfo();
  graintypeList.value = data.data;
});
</script>

<style lang="scss" scoped>
.contain {
  margin-top: 20px;
  .lab {
    display: flex;
    margin-bottom: 20px;
    align-items: center;
    .elinput {
      width: 360px;
      height: 40px;
    }
  }
  .tmp {
    display: flex;
    align-items: center;
    margin-bottom: 20px;
  }

  :deep(.el-input.is-disabled .el-input__inner) {
    color: #333333 !important;
    -webkit-text-fill-color: #333333 !important;
    cursor: not-allowed;
  }
}
</style>