<template>
  <div class="main">
    <div
      style="display: flex; align-items: center; gap: 10px; margin-bottom: 24px"
    >
      <p>种类：</p>
      <el-select
        v-model="selectValue"
        @change="selectChanged"
        size="large"
        style="width: 240px"
      >
        <el-option
          v-for="item in options"
          :key="item.value"
          :label="item.label"
          :value="item.value"
        />
      </el-select>
      <el-button type="primary" @click="dialogVisible = true">
        新增配置
      </el-button>
    </div>
    <el-card>
      <template v-for="(item, index) in tags" :key="index">
        <div class="first_type_div">
          <span class="first_type_span"> 一级分类 </span>
          <span class="first_value_span"> {{ item.value }} </span>
          <el-popconfirm
            :title="`你确定要删除${item.value}分类?`"
            width="260px"
            @confirm="deleteTags(item.id)"
          >
            <template #reference>
              <el-button
                style="
                  width: 24px;
                  height: 24px;
                  border: none;
                  margin-left: -6px;
                "
                circle
              >
                <el-icon :size="24" color="#FF0000"><Remove /></el-icon>
              </el-button>
            </template>
          </el-popconfirm>
        </div>
        <div class="first_tags">
          <template v-for="(itemsub, index) in item.items" :key="index">
            <div
              style="
                display: flex;
                align-items: center;
                gap: 10px;
                flex-wrap: wrap;
              "
            >
              <span class="first_tags_span"> 一级标签 </span>
              <span class="first_tags_value"> {{ itemsub.value }} </span>
            </div>
          </template>
        </div>
        <!-- 分割线，避免最后一个元素也有分割线 -->
        <hr v-if="index !== tags.length - 1" class="divider" />
      </template>
    </el-card>

    <el-dialog
      v-model="dialogVisible"
      title="新增配置"
      width="900"
      style="height: 700px"
      @close="resetData"
    >
      <div class="dia" style="height: 630px; overflow: auto">
        <input
          type="file"
          ref="fileInputRef"
          @change="handleFileUpload"
          accept=".json"
        />
        <el-input
          v-model="fileValue"
          style="width: 100%; margin-top: 10px; margin-bottom: 10px"
          :rows="2"
          type="textarea"
          placeholder="Please input"
          resize="none"
        >
        </el-input>
        <!-- 这里使用 Flexbox 让按钮靠右 -->
        <div
          style="
            display: flex;
            justify-content: flex-end;
            margin-top: 10px;
            margin-bottom: 30px;
          "
        >
          <el-button type="primary" @click="parseJson">解析</el-button>
        </div>
        <template v-for="(item, index) in newTags" :key="index">
          <div class="first_type_div" v-if="item.value">
            <span class="first_type_span"> 一级分类 </span>
            <span class="first_value_span"> {{ item.value }} </span>
          </div>
          <div class="first_tags" v-if="item.items">
            <template v-for="(itemsub, index) in item.items" :key="index">
              <div
                style="
                  display: flex;
                  align-items: center;
                  gap: 10px;
                  flex-wrap: wrap;
                "
              >
                <span class="first_tags_span"> 一级标签 </span>
                <span class="first_tags_value"> {{ itemsub.value }} </span>
              </div>
            </template>
          </div>
          <hr
            v-if="index !== newTags.length - 1 && item.value"
            class="divider"
          />
          <div
            style="display: flex; justify-content: flex-end; margin-top: 10px"
          >
            <el-button
              v-if="index === newTags.length - 1 && item.value"
              type="primary"
              @click="saveTags"
            >
              保存
            </el-button>
          </div>
        </template>
      </div>
    </el-dialog>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from "vue";
import { ElMessageBox } from "element-plus";
import grainTypeApi from "@/api/graintype";
import tagsApi from "@/api/tags";

const selectValue = ref<number>();
const dialogVisible = ref(false);
const fileValue = ref<string>("");
const newTags = ref<any[]>();
const fileInputRef = ref();
const tags = ref<any[]>();

const options = ref<any[]>([]);

// let tags = []; 由于 tags 不是 ref 或 reactive，Vue 不会监听它的变化

onMounted(async () => {
  let data: any = await grainTypeApi.reqTypeInfo();
  if (data.code === 200) {
    data.data.forEach((element) => {
      options.value.push({
        value: element.id,
        label: element.name,
      });
    });
    if (options.value.length > 0) selectValue.value = options.value[0].value;
    let res: any = await tagsApi.reqTagsInfo({
      typeid: selectValue.value,
      deep: 1,
    });
    tags.value = res.data;
  }
});

const handleFileUpload = (event) => {
  const file = event.target.files[0]; // 获取文件
  if (!file) return;

  const reader = new FileReader();

  reader.onload = (e) => {
    fileValue.value = e.target.result as string; // 读取到的文件内容
  };

  reader.readAsText(file); // 以文本格式读取
};

const parseJson = () => {
  try {
    let obj = JSON.parse(fileValue.value);
    if (Array.isArray(obj)) {
      newTags.value = obj;
    } else {
      ElMessageBox.alert("内容输入有误", { confirmButtonText: "确定" });
    }
  } catch (error) {
    ElMessageBox.alert("内容输入有误", { confirmButtonText: "确定" });
  }
};

const selectChanged = async () => {
  let res: any = await tagsApi.reqTagsInfo({
    typeid: selectValue.value,
    deep: 1,
  });
  tags.value = res.data;
};

const saveTags = async () => {
  let res: any = await tagsApi.reqAddTags({
    chrildobj: JSON.stringify(newTags.value),
    deep: 1,
    typeid: selectValue.value,
  });
  if (res.code !== 200) {
    ElMessageBox.alert("保存标签出错", { confirmButtonText: "确定" });
    return;
  }
  res = await tagsApi.reqTagsInfo({
    typeid: selectValue.value,
    deep: 1,
  });
  tags.value = res.data;
  dialogVisible.value = false;
};

const deleteTags = async (id: number) => {
  let res: any = await tagsApi.reqDeleteTags({ id: id });
  if (res.code !== 200) {
    ElMessageBox.alert("删除标签出错", { confirmButtonText: "确定" });
    return;
  }
  res = await tagsApi.reqTagsInfo({
    typeid: selectValue.value,
    deep: 1,
  });
  tags.value = res.data;
};

const resetData = () => {
  newTags.value = [];
  fileValue.value = "";
  fileInputRef.value.value = "";
};
</script>

<style lang="scss">
.main {
  .first_type_div {
    display: flex;
    align-items: center; /* 垂直居中 */
    gap: 15px; // 控制间距
    margin-bottom: 24px;
    .first_value_span {
      display: flex;
      justify-content: center; /* 水平居中 */
      align-items: center; /* 垂直居中 */
      background: #f1f3f4;
      border-radius: 4px;
      border: 1px solid #cbcbcb;
      width: 224px;
      height: 40px;
      color: #333333;
    }
  }
  .first_type_span {
    font-size: 14px;
    color: #333333;
  }
  .first_tags {
    margin-left: 380px;
    margin-top: -64px;
    display: flex;
    align-items: center; /* 垂直居中 */
    gap: 15px; // 控制间距
    margin-bottom: 24px;
    flex-wrap: wrap;
    .first_tags_span {
      font-size: 14px;
      color: #666666;
    }
    .first_tags_value {
      display: flex;
      justify-content: center; /* 水平居中 */
      align-items: center; /* 垂直居中 */
      background: #f1f3f4;
      border-radius: 4px;
      border: 1px solid #cbcbcb;
      width: 88px;
      height: 40px;
      color: #333333;
    }
  }
  .divider {
    border: none;
    height: 1px;
    background: #e0e0e0;
    margin: 16px 0;
  }
  .el-textarea__inner {
    height: 360px !important;
  }
  //   :deep(.el-dialog__header) {
  //     position: sticky;
  //     top: 0;
  //     background: white;
  //     z-index: 10;
  //     padding: 16px;
  //     box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
  //   }
  /* 让对话框内容部分滚动 */
  //   .el-dialog__body {
  //     overflow: auto; /* 启用滚动 */
  //   }
}
</style>