<template>
  <div class="history-container">
    <div class="tabs">
      <ul class="tab-tilte">
        <li
          v-for="(itemTit, index) in tabTitle"
          :key="index"
          @mouseover="handleMouseOver(index)"
          @mouseout="handleMouseOut(index)"
          :class="{ active: cur == index }"
        >
          {{ itemTit }}
        </li>
      </ul>
      <div class="indicatorDots">
        <span
          :class="{ active: cur == i }"
          v-for="(v, i) in tabTitle"
          :key="i"
        ></span>
      </div>
      <div class="tab-content">
        <div
          v-for="(itemCon, index) in tabCon"
          v-show="cur == index"
          :key="index"
        >
          <div v-for="(v, i) in itemCon" :key="i" class="content">
            {{ v }}
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from "vue";
const tabTitle = ref(["2020", "2021", "2022", "2023", "2024"]);
const tabCon = ref([
  [
    "2020.03，股权托管交易中心创新层挂牌并荣获“优秀成果奖”、“优秀组织奖”",
    "2020.07，合肥市首批重点产业企业",
    "2020.10，安徽省第八批信息消费创新产品",
  ],
  [
    "2021.03，工信部首台套创新产品",
    "2021.07，新产品证书",
    "2021.09，深科技、大数据企业",
    "2021.12，粮食行业会理事单位",
    "2021.12，创客大赛亚军"
  ],
  [
    "2022.05，中国数智化TOP50(前十位)",
    "2022.08，学术论文收录CVPR、论文入选SCI、论文刊登中文核心",
    "2022.10，数字质检3号产线落地、获A轮融资’",
    "2022.11，国家高新技术企业、大数据、瞪羚培育、深科技企业",
    "2022.12，创响中国大赛获奖",
  ],
  [
    "2023.03，产品入选合肥市新技术新产品新模式拟认定名单",
    "2023.08，获得美国PCT国际发明专利授权",
    "2023.08，论文被第26届欧洲人工智能会议(ECAI 2023)全文接收",
    "2023.10，获评中国粮油学会第五届优秀单位会员",
    "2023.11，论文被lEEE Robotics and Automation Letters期刊接收",
  ],
  [
    "2024.03，完成分布式计算框架和贡献证明算法的设计和开发",
    "2024.05，完成分布式计算资源动态调度和管理开发",
    "2024.10，推出链云计算生态，完成计算平台和API接口开发，完成优壹号对计算Dapp支持",
  ],
]);

let cur = ref(0);

const getTimer = () => {
  return setInterval(() => {
    cur.value++;
    if (cur.value == tabTitle.value.length) {
      cur.value = 0;
    }
  }, 200000);
};

const handleMouseOver = (index: any) => {
  cur.value = index;
//   clearInterval(getTimer());
};

const handleMouseOut = (index: any) => {
  getTimer();
};

onMounted(() => {
//   getTimer();
});

</script>

<style lang="scss" scoped>
.history-container {
  width: 100%;
  max-height: 100%;
  margin-left: 10px;
  .tabs {
    display: flex;
    .tab-tilte {
      li {
        color: #fff;
        background-color: rgba(36, 196, 255, 0.5);
        text-align: center;
        cursor: pointer;
        font-size: 20px;
        width: 100px;
        height: 40px;
        line-height: 40px;
        -webkit-border-radius: 5px;
        -ms-border-radius: 5px;
        -o-border-radius: 5px;
        -moz-border-radius: 5px;
        border-radius: 5px;
        margin-bottom: 10px;
        &.active {
          background: linear-gradient(to right, #1b81bc, 20%, #24c4ff);
          color: #fff;
        }
      }
    }

    .indicatorDots {
      background-color: rgba(36, 196, 255, 0.0);
    //   width: 10px;
    //   height: 10px;
      margin-top: 10px;
      margin-left: 10px;

      span {
        display: block;
        height: 20px;
        // margin-left: -0.0875rem;
        margin-bottom: 30px;

        &:after {
          content: "";
          display: block;
          width: 20px;
          height: 20px;
          background: #ddd;
          -webkit-border-radius: 50%;
          -ms-border-radius: 50%;
          -o-border-radius: 50%;
          -moz-border-radius: 50%;
          border-radius: 50%;
          border: 2px solid #1b81bc;
          
        }

        &.active {
          &:after {
            background: #24c4ff;
            -webkit-box-shadow: 0 0 0.125rem #1b81bc;
            box-shadow: 0 0 0.125rem #1b81bc;
          }
        }
      }
    }

    .tab-content {
      display: flex;
      align-items: center;
      
      div {
        padding: 0px;
        margin-left: 10px;
        color: #fff;
        .content {
          font-size: 20px;
          line-height: 40px;
        }
      }
    }
  }
}
</style>
